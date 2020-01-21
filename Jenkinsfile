#!/usr/bin/env groovy

pipeline {
	agent any

	stages {
		stage('Clean Workspace')	{ steps { cleanWs() } }
		stage('Checkout SCM')		{ steps { checkout scm } }
		stage('Build') {
			steps {
				cmakeBuild (
					buildDir: 'build',
					buildType: 'Debug',
					cmakeArgs: '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
					installation: 'InSearchPath',
					steps: [[
						args: 'all install',
						envVars: 'DESTDIR=${WORKSPACE}/image'
					]]
				)
			}
		}
		stage('PVS Studio analyze') {
			steps {
				withCredentials([file(credentialsId: 'PVS_STUDIO_LIC_FILE', variable: 'PVS_STUDIO_LIC_FILE')]) {
					sh """
					pvs-studio-analyzer analyze -a 61 -l ${PVS_STUDIO_LIC_FILE} -f build/compile_commands.json -o build/PVS-studio.log
					plog-converter --excludedCodes V1042 -a 'GA:1,2,3;64:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -m misra -t xml -o build/${JOB_NAME}.plog build/PVS-studio.log
					"""
				}
				recordIssues tool: PVSStudio()
			}
		}
	}
}
