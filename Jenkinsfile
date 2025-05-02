def triggers = []

if (env.BRANCH_NAME == 'develop') {
    triggers = [parameterizedCron('H 0 * * * %CLEAN_BUILD=true;BUILD=true;REPORTS=true;STATIC_ANALYSIS=true')]
}

echo 'Printing all parameters:'
params.each { param ->
    echo "${param.key} = ${param.value}"
}

echo "env.BRANCH_NAME = ${env.BRANCH_NAME}"

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
            visibleItemCount: 3,
        )
    ]),
    pipelineTriggers(triggers)
])

def nodeName = params.SPLE_BLANK_AGENT ? 'SPLE_BLANK' : 'SPLE'
// Retrieve the selected variants from the parameter
def variants = params.VARIANTS.split(',')
// Create a map to hold the parallel stages
def parallelStages = [:]
// Create an overall stage for each variant, containing all steps. This is done to avoid the need for a nested parallel stage.
// These overall stages are executed in parallel, but the steps are executed sequentially within each overall stage.
for (variant in variants) {
    def currentVariant = variant // Capture the current value of variant
    parallelStages["${currentVariant}"] = {
        node(nodeName) {
            ws("sple\\spled") {
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
                    checkout scm
                }

                stage("Installation of Dependencies - ${currentVariant}") {
                    // Initial SPLE setup
                    bat 'powershell.exe -NonInteractive -ExecutionPolicy Bypass -Command "irm https://git.marquardt.de/projects/SPLE/repos/sple-setup/raw/bin/install.ps1 | iex"'
                    bat "call build.bat ${cleanOption} -install -installOptional || exit /b 1"
                }

                stage ("Execute Tests - ${currentVariant}") {
                    // Set the filter option for the test run based on the current variant
                    def filterOption = "-filter \"${currentVariant.replaceAll('/', '__')}\""
                    // Create a list of markers for the test run
                    def testMarkers = []
                    if (params.BUILD) {
                        testMarkers.add('build')
                    }
                    if (params.REPORTS) {
                        testMarkers.add('reports')
                    }
                    if (params.STATIC_ANALYSIS) {
                        testMarkers.add('static_analysis')
                    }
                    def markerOption = testMarkers.size() > 0 ? "-marker \"${testMarkers.join(' or ')}\"" : ""
                    withCredentials([string(credentialsId: 'POLYSPACE_ACCESS_LOGIN', variable: 'POLYSPACE_ACCESS_LOGIN')]) {
                        bat "call build.bat ${cleanOption} -selftests ${filterOption} ${markerOption} || exit /b 1"
                    }
                }

                stage("Deploy Test Results - ${currentVariant}") {
                    junit allowEmptyResults: false, keepLongStdio: false, testResults: "test/output/test-report.xml,build/${currentVariant}/test/Debug/components/**/junit.xml"
                }

                stage("Deploy Artifacts - ${currentVariant}") {
                    dir('build') {
                        archiveArtifacts(
                            artifacts: "${currentVariant}/prod/Debug/spled.exe,${currentVariant}/test/Debug/reports.html,${currentVariant}/prod/Debug/reports.html"
                        )
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
