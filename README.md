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

```
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
```

## Choose port for your HTTP server

Define the HTTP_PORT in the mycode/h/myconfig.h according to your user name:
KC03Dxy -> 12000 + x*100 + HEX2DEC(y)

For example for user `KC03D1E`: 12000 + 100 + 14 = 12114

For example for user `KC03D24`: 12000 + 200 + 04 = 12204

| User ID | Port  |
| :---:   | :---: |
| KC03D12 | 12102 |
| KC03D13 | 12103 |
| KC03D14 | 12104 |
| KC03D15 | 12105 |
| KC03D16 | 12106 |
| KC03D17 | 12107 |
| KC03D18 | 12108 |
| KC03D19 | 12109 |
| KC03D1A | 12110 |
| KC03D1B | 12111 |
| KC03D1C | 12112 |
| KC03D1D | 12113 |
| KC03D1E | 12114 |
| KC03D1F | 12115 |
| KC03D21 | 12201 |
| KC03D22 | 12202 |
| KC03D23 | 12203 |
| KC03D24 | 12204 |
| KC03D25 | 12205 |
| KC03D26 | 12206 |
| KC03D27 | 12207 |
| KC03D2C | 12212 |
| KC03D3B | 12311 |
| KC03D3C | 12312 |

# Visualization part

Grafana: https://grafana.sbait.eu/
User's will be created later

## Prometheus format

By default it will go to the `/metrics` endpoint

Read more here: https://prometheus.io/docs/concepts/data_model/

General Example:
```
# HELP _parameter_ _Description_
# TYPE _parameter_ gauge|count
_parameter_{_label1_="name11",_label2_="name12",...,_labelN_="name1N"} _value1_
_parameter_{_label1_="name21",_label2_="name22",...,_labe2N_="name2N"} _value2_
```

Our case (what your application have to use):
```
job_cpu_total_ms{user="KC03Dxx",jobname="DxxHTTP"} ss.mmm
for example:
job_cpu_total_ms{user="KC03D00",jobname="WLM"} 110.005
job_cpu_total_ms{user="KC03D00",jobname="D00HTTP"} 0.002
```


HTTP handler sample:
```
  respondWithChunkedOutputStream(response);

  setResponseStatus(response, HTTP_STATUS_OK, "OK");
  setContentType(response, "text/plain");
  addStringHeader(response, "Server", "jdmfws");
  addStringHeader(response, "Transfer-Encoding", "chunked");
  writeHeader(response);

  char bigBuffer[2048];

  memset(bigBuffer, 0x00, sizeof(bigBuffer));
  sprintf(bigBuffer, "%s %s %d", "my", "test text", 1);
  writeString(response->stream, bigBuffer);

  finishResponse(response);
```

