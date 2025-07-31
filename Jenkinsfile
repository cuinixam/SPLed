def triggers = []

// Schedule a daily build trigger for the 'develop' branch at a randomized minute past midnight using Jenkins' H notation for load balancing
if (env.BRANCH_NAME == 'develop') {
    triggers = [parameterizedCron('H 0 * * * %CLEAN_BUILD=true;BUILD=true;REPORTS=true;STATIC_ANALYSIS=true')]
}

echo 'Printing all parameters:'
params.each { param ->
    echo "${param.key} = ${param.value}"
}

echo "env.BRANCH_NAME = ${env.BRANCH_NAME}"

// Extract Bitbucket organization and repository from JOB_NAME
def jobParts = env.JOB_NAME.tokenize('/')

// Validate that JOB_NAME has the expected format: "SPLE/SPLE/spled/develop"
if (jobParts.size() < 4) {
    error("Invalid JOB_NAME: '${env.JOB_NAME}'. It appears your Jenkins job does not follow the SPLE Platform's expected pattern: 'SPLE/Organisation/Repository/Branch' with 4 parts separated by '/'.")
}

def platformNode = jobParts[1]
def platformOrg = jobParts[1]
def platformRepo = jobParts[2]
echo "PLATFORM_NODE = $platformNode"
echo "PLATFORM_ORG = $platformOrg"
echo "PLATFORM_REPO = $platformRepo"

properties([
    buildDiscarder(logRotator(numToKeepStr: '20')),
    disableConcurrentBuilds(),
    // Defaults are for builds triggered by SCM events (push, creation of branch or pull request)
    parameters([
        booleanParam(
            defaultValue: false,
            description: 'Run a clean build?',
            name: 'CLEAN_BUILD'
        ),
        booleanParam(
            defaultValue: true,
            description: 'Run target build',
            name: 'BUILD'
        ),
        choice(
            choices: ['Debug', 'Release'],
            description: 'Build type of the target build',
            name: 'BUILD_TYPE'
        ),
        booleanParam(
            defaultValue: true,
            description: 'Run unit tests and create reports',
            name: 'REPORTS'
        ),
        booleanParam(
            defaultValue: false,
            description: 'Run static code analysis',
            name: 'STATIC_ANALYSIS'
        ),
        booleanParam(
            defaultValue: false,
            description: 'Perform a build on a blank SPLE agent?',
            name: 'SPLE_BLANK_AGENT'
        ),
        extendedChoice(
            name: 'VARIANTS',
            description: 'Select the variants to run the CI tests for',
            type: 'PT_MULTI_SELECT',
            multiSelectDelimiter: ',',
            value: 'Disco,Sleep,Spa,Base/Dev',
            defaultValue: 'Disco,Sleep,Spa,Base/Dev',
            visibleItemCount: 4,
        )
    ]),
    pipelineTriggers(triggers)
])

def nodeName = params.SPLE_BLANK_AGENT ? 'SPLE_BLANK' : platformNode

// Retrieve the selected variants from the parameter
def variants = params.VARIANTS.split(',')

// Retrieve the build type from the parameters
def buildType = params.BUILD_TYPE ? params.BUILD_TYPE : ''

// Create a map to hold the parallel stages
def parallelStages = [:]

// Create an overall stage for each variant, containing all steps. This is done to avoid the need for a nested parallel stage.
// These overall stages are executed in parallel, but the steps are executed sequentially within each overall stage.
for (variant in variants) {
    def currentVariant = variant // Capture the current value of variant
    parallelStages["${currentVariant}"] = {
        node(nodeName) {
            workspaceSuffix = getWorkspaceSuffix()
            ws("$platformOrg/$platformRepo/${workspaceSuffix}") {
                // Set the clean build option based on the parameter
                def cleanOption = params.CLEAN_BUILD ? "-clean" : ""
                if (params.SPLE_BLANK_AGENT) {
                    // currently there is no blank agent available in our Jenkins instance
                    // so we have to clean the agent manually
                    // 1. clean workspace
                    cleanWs()
                    // 2. wipe any previous installations of the SPLE Platform Toolset
                    bat '''
if exist "%USERPROFILE%\\.sple-setup" rmdir /s /q "%USERPROFILE%\\.sple-setup"
if exist "%USERPROFILE%\\scoop" rmdir /s /q "%USERPROFILE%\\scoop"
'''
                }

                stage("Checkout Code - ${currentVariant}") {
                    // git should use the Windows Store (certificates), but this fails sometimes
                    bat 'git config --global http.sslVerify false'

                    // checkout the code and branch to be tested
                    checkout scm

                    if (params.CLEAN_BUILD || params.SPLE_BLANK_AGENT) {
                        // Clean the workspace
                        bat 'git clean -ffdx'
                    }
                }

                stage("Installation of Dependencies - ${currentVariant}") {
                    // Initial SPLE setup
                    bat 'powershell.exe -NonInteractive -ExecutionPolicy Bypass -Command "irm https://git.marquardt.de/projects/SPLE/repos/sple-setup/raw/bin/install.ps1 | iex"'
                    bat "call build.bat -install -installOptional || exit /b 1"
                }

                stage ("Execute Tests - ${currentVariant}") {
                    // Set the filter option for the test run based on the current variant
                    def filterOption = "-filter \"${currentVariant.replaceAll('/', '__')}\""
                    // Create a list of markers for the test run
                    def testMarkers = []
                    if (params.BUILD) {
                        if (buildType?.trim()) {
                            testMarkers.add("build_${buildType.toLowerCase()}")
                        }
                        else {
                            testMarkers.add('build')
                        }
                    }
                    if (params.REPORTS) {
                        testMarkers.add('reports')
                    }
                    if (params.STATIC_ANALYSIS) {
                        testMarkers.add('static_analysis')
                    }
                    def markerOption = testMarkers.size() > 0 ? "-marker \"${testMarkers.join(' or ')}\"" : ""

                    withCredentials([string(credentialsId: 'POLYSPACE_ACCESS_LOGIN', variable: 'POLYSPACE_ACCESS_LOGIN')]) {
                        withEnv(["PLATFORM_NODE=$platformNode", "PLATFORM_ORG=$platformOrg", "PLATFORM_REPO=$platformRepo"]) {
                            bat "call build.bat ${cleanOption} -selftests ${filterOption} ${markerOption} || exit /b 1"
                        }
                    }
                }

                stage("Deploy Test Results - ${currentVariant}") {
                    // Evaluate overall quality gate checks (pytest results)
                    junit allowEmptyResults: false, keepLongStdio: false, testResults: "test/output/test-report.xml"

                    // Evaluate unit, component and integration test results (GoogleTest)
                    junit allowEmptyResults: false, keepLongStdio: true, testResults: "build/${currentVariant}/test/Debug/components/**/junit.xml"
                }

                stage("Deploy Artifacts - ${currentVariant}") {
                    dir('build') {
                        script {
                            def build_folder_prod = "${currentVariant}/prod"
                            def build_folder_test = "${currentVariant}/test"
                            if (buildType?.trim()) {
                                // Use the specified build type if provided
                                build_folder_prod = "${build_folder_prod}/${buildType}"
                                build_folder_test = "${build_folder_test}/${buildType}"
                            }
                            echo "Using build folder for production: ${build_folder_prod}"
                            echo "Using build folder for test: ${build_folder_test}"

                            // Define artifact patterns
                            def artifactPatterns = [
                                // Binary files
                                "**/${build_folder_prod}/**/*.elf",
                                "**/${build_folder_prod}/**/*.bin",
                                "**/${build_folder_prod}/**/*.hex",
                                "**/${build_folder_prod}/**/*.s19",
                                // Map and debug files
                                "**/${build_folder_prod}/**/*.map",
                                "**/${build_folder_prod}/**/*.mdf",
                                // Security and checksums
                                "**/${build_folder_prod}/**/*.crc",
                                "**/${build_folder_prod}/**/*.rsa",
                                // ODX files
                                "**/${build_folder_prod}/**/*.odx",
                                "**/${build_folder_prod}/**/*.pdx",
                                "**/${build_folder_prod}/**/*.odx-f",
                                "**/${build_folder_prod}/**/*.ouc",
                                // Flash packages
                                "**/${build_folder_prod}/**/*.vflashpack",
                                // A2L files
                                "**/${build_folder_prod}/**/a2l/**/*.a2l",
                                "**/${build_folder_prod}/**/a2l/**/*.log",
                                // Release artifacts
                                "**/release/**/*",
                                // Reports and logs
                                "**/${build_folder_prod}/**/resource_analysis_report.json",
                                "**/${build_folder_test}/reports.html",
                                "**/${build_folder_prod}/**/build.log",
                                "**/${build_folder_test}/reports.log",
                                // Variant-specific artifacts with dynamic build type
                                "**/${build_folder_prod}/**/*.exe",
                                "**/${build_folder_prod}/reports.html"
                            ]

                            archiveArtifacts(
                                artifacts: artifactPatterns.join(','),
                                excludes: 'deps/**/*,**/CMakeFiles/**/*',
                            )
                        }
                    }
                }
            }
        }
    }
}

// Execute the overall stages in parallel
stage('Parallel Variants') {
    parallel parallelStages
}

// To avoid any "long paths" problems we create a hash out of the release branch
// to shorten the length of the workspace path by using the first 8 chars of
// the SHA-1 hash (similar to git commits).
def getWorkspaceSuffix() {
    def suffix = 'spled' // default suffix for non-release branches and PRs

    // adapt workspace for release branches and PRs with a release branch as target
    def branchName = ''
    if ( (env.CHANGE_ID == null) && env.BRANCH_NAME.startsWith('release/') ) {
        branchName = env.BRANCH_NAME
    }
    if ( (env.CHANGE_ID != null) && env.CHANGE_TARGET.startsWith('release/') ) {
        branchName = env.CHANGE_TARGET
    }
    if (branchName) {
        def gitUsrPath = 'c:\\Git\\usr\\bin'
        suffix = bat(returnStdout: true, script: """@echo off
echo ${env.BRANCH_NAME} | ${gitUsrPath}\\sha1sum.exe | ${gitUsrPath}\\cut -c 1-8
""").trim()
    }
    return suffix
}
