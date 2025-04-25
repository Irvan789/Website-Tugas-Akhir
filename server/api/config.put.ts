import { deviceInitConfig } from "~/utils/InitialData"

export default defineEventHandler(async (event) => {
  const config = useRuntimeConfig()

  try {
    const deviceConfig = await deviceRef.get()

    if (deviceConfig.exists()) {
      throw new Error("Konfigurasi Sudah Ada")
    }

    await deviceRef.set(deviceInitConfig)
    await statisticRef.update({
      configTime: deviceInitConfig.configTime
    })

    return responseSuccess(event, 200, "Berhasil Menginisiasi Konfigurasi!")
  } catch (e) {
    if (e instanceof Error) {
      return responseError(400, e.message)
    }
  }
})
