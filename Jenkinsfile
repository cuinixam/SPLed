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
    // Defaults are for PR (Pull Request) builds
    // see parameterizedCron settings above for branch specific settings
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
        )
    ]),
    pipelineTriggers(triggers)
])

node('SPLE') {
    ws('sple/spled') {
        def cleanOption = params.CLEAN_BUILD ? "-clean" : ""

        stage('Checkout Code') {
            // git should use the Windows Store (certificates), but this fails sometimes
            bat 'git config --global http.sslVerify false'
            checkout scm
        }

        stage('Installation of Dependencies') {
            // Initial SPLE setup
            bat 'powershell.exe -NonInteractive -ExecutionPolicy Bypass -Command "irm https://git.marquardt.de/projects/SPLE/repos/sple-setup/raw/bin/install.ps1 | iex"'
            bat "call build.bat ${cleanOption} -install -installOptional || exit /b 1"
        }

        stage('Execute Tests') {
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
                bat "call build.bat ${cleanOption} -selftests ${markerOption} || exit /b 1"
            }
        }

        stage('Deploy Test Results') {
            junit allowEmptyResults: false, keepLongStdio: false, testResults: 'test/output/test-report.xml,build/**/test/src/**/junit.xml'
        }

        stage('Deploy Artifacts'){
            dir('build') {
                archiveArtifacts(
                    artifacts: '**/prod/spled.exe,**/test/reports.html,**/prod/reports.html'
                )
            }
        }
    }
}
