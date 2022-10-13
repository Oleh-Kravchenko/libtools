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
	}
}
