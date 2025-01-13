def triggers = []
def param_default_static_analysis = false

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
        stage('init') {
            // git should use the Windows Store (certificates), but this fails sometimes
            bat 'git config --global http.sslVerify false'
            checkout scm

            // Initial SPLE setup
            bat 'powershell.exe -NonInteractive -ExecutionPolicy Bypass -Command "irm https://git.marquardt.de/projects/SPLE/repos/sple-setup/raw/bin/install.ps1 | iex"'
        }

        stage('test') {
            // Build and deploy docs
            def additionalOptions = params.CLEAN_BUILD ? "-clean" : ""

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

            bat """
call build.bat ${additionalOptions} -install -installOptional || exit /b 1
call build.bat ${additionalOptions} -selftests -marker "${testMarkers.join(' or ')}" || exit /b 0
"""

            junit allowEmptyResults: false, keepLongStdio: false, testResults: 'test/output/test-report.xml,build/**/test/src/**/junit.xml'
        }

        dir('build') {
            archiveArtifacts(
                artifacts: '**/prod/spled.exe,**/test/reports.html,**/prod/reports.html'
            )
        }
    }
}
