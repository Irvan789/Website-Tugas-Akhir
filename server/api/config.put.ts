import { deviceInitConfig } from "~/utils/InitialData"

export default defineEventHandler(async (event) => {
  try {
    const deviceConfig = await deviceRef.get()

    if (deviceConfig.exists()) {
      throw new Error("Konfigurasi Telah Terinisiasi!")
    }

    await deviceRef.set(deviceInitConfig)

    return responseSuccess(event, 200, "Berhasil Menginisiasi Konfigurasi!")
  } catch (e) {
    if (e instanceof Error) {
      return responseError(400, e.message)
    }
  }
})
