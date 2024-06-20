# Fabric Sync Demo Setup

## Setup Fabric Source

Connect to the Fabric Source server:

```
ssh ubuntu@192.168.0.103
```

Password: test1234

Run the Fabric Source script:

```
./run_fabric_source.sh
```

## Setup Fabric Sink

Connect to the Fabric Sink server:

```
ssh ubuntu@192.168.0.102
```

Password: test1234

Run the Fabric Sink script:

```
./run_fabric_sink.sh
```

## Fabric Sync Setup

Enable Fabric Auto Sync:

In Fabric-Sync console:

```
fabricsync enable-auto-sync 1
```

Pair the Fabric-Source bridge to Fabric-Sync with node ID 1:

```
fabricsync add-bridge 1 192.168.0.103
```

## Start Lighting Example App

Connect to the Lighting Example server:

```
ssh ubuntu@192.168.0.106
```

Password: test1234

Run the Lighting Example script:

```
./run_light_example.sh
```

## Pair Light Example to Fabric-Source

Pair the Light Example using its payload number:

```
pairing already-discovered 3 20202021 192.168.0.106 5540
```

After the Light Example is successfully paired in Fabric-Source, it will be
synced to Fabric-Sink with a new assigned node ID.

Toggle the Light Example:

From Fabric-Source:

```
onoff on 3 1
onoff off 3 1
```

From Fabric-Sink: (Use the node ID assigned)

```
onoff on x 1
onoff off x 1
```

## Remove Light Example from Fabric-Source

Unpair the Light Example:

```
pairing unpair 3
```

After the Light Example is successfully unpaired from Fabric-Source, it will
also be removed from the Fabric-Sink.

## Pair Tapo Switch to Fabric-Source

Pair the Tapo Switch using its payload number:

In Fabric-Source console:

```
pairing code-wifi 4 yufeng-test 8204test 03414841859
```

After the Tapo Switch is successfully paired in Fabric-Source, it will be synced
to Fabric-Sink with a new assigned node ID.

Toggle the Tapo Switch:

From Fabric-Source:

```
onoff on 4 1
onoff off 4 1
```

From Fabric-Sink: (Use the node ID assigned)

```
onoff on x 1
onoff off x 1
```

## Remove Tapo Switch from Fabric-Source

Unpair the Tapo Switch:

```
pairing unpair 4
```

After the Tapo Switch is successfully unpaired from Fabric-Source, it will also
be removed from the Fabric-Sink.
