# Project Tugas Akhir

## Tentang

Website ini dibangun menggunakan framework nuxt dan firebase untuk melakukan monitoring dan kontrolling perangkat IoT.

## IoT API Endpoint

### Base URL

```
/api/iot
```

**Required Params**

| Parameter | Type   | Description                                     |
| :-------- | :----- | :---------------------------------------------- |
| deviceId  | string | Kode Unik Perangkat **(Required)**              |
| key       | string | API Key Untuk Membuat Permintaan **(Required)** |

### Get Device Config & Time

```http
GET /config
```

Response:

```json
{
  "statusCode": 200,
  "data": {
    "automated": true,
    "configTime": 1729598802,
    "pln": {
      "active": true
    },
    "plts": {
      "active": true,
      "min_voltage": 220
    },
    "relay": {
      "n1": {
        "source": "auto",
        "max_load": 50
      },
      "n2": {
        "source": "auto",
        "max_load": 70
      },
      "n3": {
        "source": "auto",
        "max_load": 100
      }
    }
  }
}
```

Catatan:

1. Key configTime pada object data menunjukkan waktu terakhir konfigurasi diubah.

### Store Sensor Data

```http
PUT /sensor
```

Payload:

```json
{
  "vpln": 230,
  "vplts": 230,
  "vbatt": 13.2,
  "lpln": 75,
  "lplts": 25,
  "relay": {
    "n1": {
      "source": "plts",
      "load": 25
    },
    "n2": {
      "source": "pln",
      "load": 35
    },
    "n3": {
      "source": "pln",
      "load": 40
    }
  }
}
```

Response:

```json
{
  "statusCode": 200,
  "data": {
    "configTime": 1729598802
  }
}
```

Catatan:

1. Key configTime pada object data menunjukkan waktu terakhir konfigurasi diubah.
