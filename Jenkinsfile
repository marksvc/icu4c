#!groovy
// Copyright (c) 2016 SIL International
// This software is licensed under the MIT license (http://opensource.org/licenses/MIT)

ansiColor('xterm') {
    timestamps {
        properties([
            // Add buildKind parameter
            parameters([choice(name: 'buildKind', choices: 'Continuous\nRelease',
                description: 'Is this a continuous (pre-release) or a release build?')]),
            // Add Gerrit Trigger
            pipelineTriggers([gerrit(gerritBuildFailedVerifiedValue: -1, gerritBuildSuccessfulVerifiedValue: 1,
                gerritBuildUnstableCodeReviewValue: -1,
                gerritProjects: [[branches: [[compareType: 'PLAIN', pattern: env.BRANCH_NAME]],
                compareType: 'PLAIN', disableStrictForbiddenFileVerification: false, pattern: 'icu4c']],
                serverName: 'defaultServer', triggerOnEvents: [
                    patchsetCreated(excludeDrafts: false, excludeNoCodeChange: true, excludeTrivialRebase: false),
                    refUpdated()
                ])])
        ])

        // Set default. This is only needed for the first build.
        buildKind = buildKind ?: 'Continuous'

        try {
            isGerritChange = GERRIT_CHANGE_NUMBER ? true : false
        }
        catch(err) {
            isGerritChange = false
        }

        node('windows && supported') {
            def msbuild = tool 'msbuild12'
            def git = tool(name: 'Default', type: 'git')
            echo "env['GERRIT_CHANGE_NUMBER']=${env['GERRIT_CHANGE_NUMBER']}"

            stage('Checkout') {
                checkout scm

                // Workaround until gerrit-trigger plugin allows to checkout change directly
                // (https://wiki.jenkins-ci.org/display/JENKINS/Gerrit+Trigger#GerritTrigger-PipelineJobs)
                // Fetch the changeset to a local branch using the build parameters provided to the
                // build by the Gerrit plugin...
                if (isGerritChange) {
                    def changeBranch = "change-${GERRIT_CHANGE_NUMBER}-${GERRIT_PATCHSET_NUMBER}"
                    bat """
                        "${git}" fetch origin ${GERRIT_REFSPEC}:${changeBranch}
                        "${git}" checkout ${changeBranch}"
                        """
                }

                // We expect that the branch name contains the ICU version number, otherwise default to 54
                def IcuVersion = (env.BRANCH_NAME =~ /[0-9]+/)[0] ?: 54
                def PreRelease = isGerritChange ? "-ci" : (buildKind != 'Release' ? "-beta" : "")
                PkgVersion = "${IcuVersion}.1.${BUILD_NUMBER}${PreRelease}"

                currentBuild.displayName = PkgVersion
            }

            dir("nugetpackage") {
                dir("build") {
                    stage('Build ICU') {
                        echo "Compiling ICU"
                        /*bat """
                            "${msbuild}" /t:Build
                            """
                            */
                    }

                    stage('Pack nuget') {
                        echo "Creating nuget package ${PkgVersion}"
                        bat """
                            "${msbuild}" /t:BuildPackage /p:PkgVersion=${PkgVersion}
                            """
                    }
                }

                if (!isGerritChange) {
                    archiveArtifacts "*.nupkg"
                }

                setGerritReview()
            }
        }

        /*
        node('master') {
            // workaround for a problem with gerrit trigger plugin:
            // I couldn't get it to report back the verified/code-review results, it only reported
            // a comment but didn't set the result. So we explicitly set the results.
            if (currentBuild.Result == "SUCCESS") {
                verified = 1
                codereview = 0
            } else if (currentBuild.Result == "FAILED") {
                verified = -1
                codereview = 0
            } else if (currentBuild.Result == "UNSABLE") {
                verified = 0
                codereview = -1
            }
            else {
                verified = 0
                codereview = 0
            }
            sh "ssh -p 59418 gerrit.lsdev.sil.org gerrit ${env['GERRIT_CHANGE_NUMBER']},${env['GERRIT_PATCHSET_NUMBER']} --verified ${verified} --code-review ${codereview}"
        }
        */
    }
}