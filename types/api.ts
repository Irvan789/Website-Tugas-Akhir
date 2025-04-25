import type { IDevices } from "~/types/database"

interface APIResponse {
  statusCode: number
  message?: string
}

export interface IoTDeviceConfigAPI extends APIResponse {
  data: IDevices | null
}

export interface IoTSensorsDataAPI extends APIResponse {
  data: {
    configTime: number
  }
}
