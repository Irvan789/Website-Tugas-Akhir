# Project Tugas Akhir

## Tentang

Repo ini merupakan keseluruhan sistem yang dirancang oleh saya sendiri. Di dalam repo ini terdapat kodingan mikrokontroller esp32, website, serta skripsi yang dibuat. Semoga repo ini dapat bermanfaat sebagai referensi dan dapat lebih dikembangkan lagi.

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
        "max_load": 25
      },
      "n2": {
        "source": "auto",
        "max_load": 40
      },
      "n3": {
        "source": "auto",
        "max_load": 60
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
      "power": 25,
      "current": 0.114,
      "source": "plts"
    },
    "n2": {
      "power": 35,
      "current": 0.159,
      "source": "pln"
    },
    "n3": {
      "power": 40,
      "current": 0.182,
      "source": "pln"
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

1. Key configTime pada object data menunjukkan waktu terakhir konfigurasi diperbarui.
