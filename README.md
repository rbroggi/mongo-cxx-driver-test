
# Mongo db experience.

Overral dependency:
  the mongodb cxx driver is found under the mongo-cxx-driver/ folder
  the mongodb cxx driver depends on the mongo-c-driver which is installed under mongo-c-driver/install
  the mongodb server (mongod) is installed under mongodb-osx-x86_64-4.0.3

To test the mongo-cxx-driver one must install the mongo-c-driver:

1. mongo-c-driver installation (local installation)

    a. download and extract the mongo-c-driver source code  
    ```sh
    $ wget https://github.com/mongodb/mongo-c-driver/releases/download/1.13.0/mongo-c-driver-1.13.0.tar.gz  
    $ tar xzf mongo-c-driver-1.13.0.tar.gz  
    ```
    
    b. enter the build directory  
    ```sh
    $ cd mongo-c-driver-1.13.0/cmake-build/  
    ```
    
    c. create the installation local folder  
    ```sh
    $ mkdir /Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-c-driver-1.13.0/install  
    ```
    
    d. configure the build procedure (cmake)  
    ```sh
    $ cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_INSTALL_PREFIX=/Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-c-driver-1.13.0/install ..  
    ```
    
    e. build (make)  
    ```sh
    $ make  
    ```
    
    f. install  
    ```sh
    $ make install  
    ```
  
2. mongo-cxx-driver installation (local installation)  
  
    a. download the mongo-cxx-driver source code:  
    ```sh
    $ git clone https://github.com/mongodb/mongo-cxx-driver.git  
    ```
    
    b.enter the build directory  
    ```sh
    $ cd mongo-cxx-driver/build  
    ```
    
    c.create the installation local folder  
    ```sh
    $ mkdir /Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-cxx-driver/install  
    ```
    
    d.configure the build procedure (cmake)  
    ```sh
    $ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-cxx-driver/install -DCMAKE_PREFIX_PATH=/Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-c-driver-1.13.0/install ..  
    ```
    
    e.build (make)  
    ```sh
    $ make  
    ```
    
    f.install  
    ```sh
    $ make install  
    ```
  
3. mongodb server install (mongod and tools)  

    a. download latest version from:  
    ```sh
    https://docs.mongodb.com/manual/tutorial/install-mongodb-on-os-x/  
    ```
    
    b. extract archive and place it under the directory where you want it to be  
    c. start the daemon with the data folder you prefer  
    ```sh
    nohup <extracted_path>/bin/mongod -dbpath <data_path> &  
    ```
    e.g.: 
    ```sh
    nohup mongod -dbpath /Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongodb-osx-x86_64-4.0.3/data/db &)  
    ```
  
4. Testing cxx driver with running mongodb  
  
    a. Start the mongodb daemon as in point 3.c  
    b. Create cmake project as you can fine in CMakeLists.txt under mongo_test  
    c. use CMAKE_PREFIX_PATH=/path/where/cxxdriver/has/been/installed  
    e.g: /Users/rodrigobroggi/Documents/Workspace/cpp/mongo/mongo-cxx-driver/install
