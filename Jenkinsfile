properties([
    buildDiscarder(logRotator(numToKeepStr: '20')),
    disableConcurrentBuilds(),
    parameters([
        booleanParam(
            defaultValue: false,
            description: 'Perform a clean build?',
            name: 'CLEAN_BUILD'
        )
    ]),
    pipelineTriggers([
        cron('H 0 * * *')
    ])
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

            def testMarkers = "build or reports"
            if(currentBuild.rawBuild.getCause(hudson.triggers.TimerTrigger.TimerTriggerCause))
            {
                testMarkers += " or static_analysis"
            }

            bat """
call build.bat ${additionalOptions} -install -installOptional || exit /b 1
call build.bat ${additionalOptions} -selftests -marker "${testMarkers}" || exit /b 0
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
