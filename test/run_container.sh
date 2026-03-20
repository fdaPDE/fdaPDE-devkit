#/bin/bash

testing=$(pwd)/../

docker run --rm -v $testing:/root -ti aldoclemente/fdapde-docker:latest /bin/bash 

