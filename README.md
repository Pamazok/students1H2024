# students1H2024
Information for students

## System information

Marist z/OS: zos.kctr.marist.edu
z/OSMF port: 10443

# Metal C stuff

## Prerequisites

For Metal C code we need kind of "library", so Zowe will be our choice.
Latest Zowe releases have lots of extra stuff, so we could take one of oldest release, like:
```bash
git clone https://github.com/zowe/zowe-common-c.git -b zss-v1.1.0
```
Or 
```bash
git clone https://github.com/zowe/zowe-common-c.git
cd zowe-common-c
git checkout zss-v1.1.0
```

## Structure

.
├── zowe-common-c           # Zowe directory
|   ├── c                   # Zowe Sources
|   └── h                   # Zowe Headers
├── mycode                  # Directory where we will store our changes
|   ├── c                   # Our Sources
|   |   ├── test.c          # File for testing your code (support LE and METAL)
|   |   └── myhttp.c        # File for our HTTP server
|   └── h                   # Our Headers
|       └── myconfig.h      # Configuration header unique for your own user
|   ├── assembler.sh        # Small script to Assemble the C sources
|   ├── build.sh            # Script to run build (ussTest,metalTest,metalHTTP)
|   ├── compile64.sh        # Small script to Compile the C sources
|   ├── link.sh             # Small script to Link the C sources
|   ├── metalHTTP.jcl       # JCL sample to run the compiled Metal C HTTP server
|   └── metalTest.jcl       # JCL sample to run the compiled Metal C TEST code
└── README.md

## Choose port for your HTTP server

Define the HTTP_PORT in the mycode/h/myconfig.h according to your user name:
KC03Dxy -> 12000 + x*100 + HEX2DEC(y)
For example for user `KC03D1E`: 12000 + 100 + 14 = 12114
For example for user `KC03D24`: 12000 + 200 + 04 = 12204

# Visualization part

Grafana: TBD

## Prometheus format

By default it will go to the `/metrics` endpoint 
Read more here: https://prometheus.io/docs/concepts/data_model/

