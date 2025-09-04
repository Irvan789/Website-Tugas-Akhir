# Project Tugas Akhir

## Tentang

Repo ini merupakan keseluruhan sistem yang dirancang oleh saya sendiri. Di dalam repo ini terdapat kodingan mikrokontroller esp32, website, serta skripsi yang dibuat. Semoga repo ini dapat bermanfaat sebagai referensi dan dapat lebih dikembangkan lagi.

## IoT API

### Base URL

```http
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
    "config": {
      "automated": true,
      "pln": {
        "active": true
      },
      "plts": {
        "active": true,
        "battery": {
          "min_voltage": 10.8
        }
      },
      "relay": {
        "n1": {
          "source": "auto",
          "max_load": 0.35
        },
        "n2": {
          "source": "auto",
          "max_load": 0.65
        },
        "n3": {
          "source": "auto",
          "max_load": 1
        }
      },
      "time": 1729598802
    },
    "time": 1729598802
  }
}
```

Catatan:

1. Key time pada object config menunjukkan waktu terakhir konfigurasi diubah.
2. Key time pada object data menunjukkan waktu sekarang yang diberikan oleh server.

### Store Sensor Data

```http
PUT /sensor
```

Payload:

```json
{
  "pln": {
    "voltage": 220,
    "load": 1
  },
  "plts": {
    "voltage": 218,
    "load": 0.5,
    "battery": {
      "voltage": 10.8
    }
  },
  "relay": {
    "n1": {
      "source": "pln",
      "load": 0.1
    },
    "n2": {
      "source": "plts",
      "load": 0.25
    },
    "n3": {
      "source": "plts",
      "load": 0.45
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
