
I. Running x86 Linux

1. Build Fabric Admin on the receving side.
./scripts/examples/gn_build_example.sh examples/fabric-admin out/debug/standalone chip_config_network_layer_ble=false


2. Build both Fabric Admin & Fabric Bridge on the sending side.
./scripts/examples/gn_build_example.sh examples/fabric-admin out/debug/standalone chip_config_network_layer_ble=false 'import("//with_pw_rpc.gni")'
./scripts/examples/gn_build_example.sh examples/fabric-bridge-app/linux out/debug/standalone chip_config_network_layer_ble=false 'import("//with_pw_rpc.gni")'


3. Start both Fabric Admin & Fabric Bridge on the sending side

yufengw@yufengw:~/connectedhomeip/out/debug/standalone$ ./fabric-bridge-app
yufengw@yufengw:~/connectedhomeip/out/debug/standalone$ ./fabric-admin

You should see Fabric Admin & Fabric Bridge are connected
>>> INF  [1718674161.727749][604759:604762] CHIP:-: Connected to Fabric-Bridge
>>> INF  [1718674162.004992][604755:604755] CHIP:-: Connected to Fabric-Admin

4. Start Fabric Admin on the receiving side
yufengw@yufengw-MX70:~/connectedhomeip/out/debug/standalone$ ./fabric-admin

Add the sending Fabric Bridge and enable the Fabric Auto-Sync on (Make sure no other device is broadcasting in the network except the Fabric Bridge)

>>> fabricsync add-bridge 1 (1 is the nodeid assigned to the bridge)
>>> fabricsync enable-auto-sync 1
[1718674478.006479][68190:68190] CHIP:-: Command: fabricsync enable-auto-sync 1 
Auto Fabric Sync is enabled.


5. Now you can test Fabric Sync

5.1 Adding new device
Start a lighting example and commision it using the Fabric Admin on the sending side
pairing onnetwork 3 20202021  (3 is the new nodeid, could be any number, but it needs to be different from the nodeid assigned to the Fabric Bridge)

5.2 Removing paired device
pairing unpair 3

Then you should see the device also get added/removed from the receiving fabric


----------------------------------------------------------------------------------------------------------------------------------------------------------


II. Running Raspberry Pi 4

1. Pull Docker Images
docker pull connectedhomeip/chip-build-vscode:latest
docker images


2. Run docker
docker run -it -v ~/connectedhomeip:/var/connectedhomeip connectedhomeip/chip-build-vscode:latest /bin/bash

3. Build
cd /var/connectedhomeip

git config --global --add safe.directory /var/connectedhomeip
git config --global --add safe.directory /var/connectedhomeip/third_party/pigweed/repo
git config --global --add safe.directory /var/connectedhomeip/examples/common/QRCode/repo

./scripts/run_in_build_env.sh \
 "./scripts/build/build_examples.py \
    --target linux-arm64-light-clang \
    build"

./scripts/run_in_build_env.sh \
 "./scripts/build/build_examples.py \
    --target linux-arm64-fabric-admin-clang \
    build"

./scripts/run_in_build_env.sh \
 "./scripts/build/build_examples.py \
    --target linux-arm64-fabric-admin-clang-rpc \
    build"

./scripts/run_in_build_env.sh \
 "./scripts/build/build_examples.py \
    --target linux-arm64-fabric-bridge-no-ble-clang-rpc \
    build"


4. Transferring a file to a Raspberry Pi
scp ./fabric-admin ubuntu@192.168.0.102:/home/ubuntu
scp ./fabric-bridge-app ubuntu@192.168.0.102:/home/ubuntu


/home/ubuntu
Writable: Yes
Persistent: Yes

/tmp
Writable: Yes
Persistent: No
