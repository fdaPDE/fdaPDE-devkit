# Getting started

Clone this repository by running:
```
git clone --recurse-submodules git@github.com:aldoclemente/fdapde-starter.git
```
A Docker image is provided to compile and run the C++ library [fdaPDE-cpp](https://github.com/fdaPDE/fdaPDE-cpp) or its [core](https://github.com/fdaPDE/fdaPDE-core).
Additional details are available at [fdapde-docker](https://github.com/aldoclemente/fdaPDE-docker). 
Pull the latest image:
```
docker pull aldoclemente/fdapde-docker:latest
```

This script updates all submodules to their tracked versions.
```
./update.sh
```

