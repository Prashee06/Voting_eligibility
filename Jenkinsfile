pipeline {
    agent any

    stages {

        stage('Checkout') {
            steps {
                echo 'Checking out source code from GitHub...'
                checkout scm
            }
        }

        stage('Build C++ Application') {
            steps {
                echo 'Compiling Voting Eligibility Checker...'

                bat '''
                    g++ server.cpp -o server.exe -std=c++17 -pthread -lws2_32
                '''
            }
        }

        stage('Docker Build') {
            steps {
                echo 'Building Docker image...'

                bat '''
                    docker build -t voting-eligibility:latest .
                '''
            }
        }

        stage('Docker Test') {
            steps {
                echo 'Testing Docker image...'

                bat '''
                    docker rm -f voting-jenkins-test 2>nul || exit /b 0
                    docker run -d -p 9091:9090 --name voting-jenkins-test voting-eligibility:latest
                    timeout /t 5 /nobreak
                    docker ps
                    docker rm -f voting-jenkins-test
                '''
            }
        }
    }

    post {
        success {
            echo 'Voting Eligibility Checker CI/CD pipeline completed successfully!'
        }

        failure {
            echo 'Pipeline failed. Check the console output.'
        }
    }
}