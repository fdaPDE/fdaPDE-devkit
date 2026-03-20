## Run Simulations

Use the `aldoclemente/fdapde-docker:latest` Docker image to compile and run C++ scripts.

### 1. Pull the Docker image

```bash
docker pull aldoclemente/fdapde-docker:latest
```

### 2. Start the container

The following script mounts `fdaPDE-devkit` into an interactive Docker container.

```bash
# the working directory must be fdaPDE-devkit/test/
./run_container.sh
```

### 3. Navigate to the test directory inside the container

The repository is mounted in `$HOME` (`/root`) inside the container.

```bash
cd $HOME/test
```

### 4. Compile and run a C++ script

To compile `script.cpp`, run:

```bash
./compile.sh script.cpp
```

This script builds an executable with the same name as the source file, without the `.cpp` extension.

Run the executable as usual:

```bash
./script
```

### 5. Exit the container

Once you have finished working inside the Docker container, run:

```bash
exit
```


## Keep your work organized

Keep quantitative and qualitative results well organized and clearly associated with the script that produced them.

See the example script `azzimonti_test_C.cpp`, which automatically creates an output directory where results are stored. We recommend following this structure.
