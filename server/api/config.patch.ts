import moment from "moment"
import { ZodError } from "zod"

import { zConfigValidation } from "~/utils/Validation"
import { responseError, responseSuccess } from "~/server/utils/ApiResponse"

import { IDevices } from "~/types/database"

export default defineEventHandler(async (event) => {
  const config = useRuntimeConfig()
  const data: IDevices = await readBody(event)

  try {
    await zConfigValidation.parseAsync(data)

    const timeUnix = moment().unix()

    await deviceRef.update({
      automated: data.automated,
      pln: data.pln,
      plts: data.plts,
      relay: data.relay,
      configTime: timeUnix
    })

    await statisticRef.update({
      configTime: timeUnix
    })

    return responseSuccess(event, 200, "Berhasil Memperbarui Konfigurasi!")
  } catch (e) {
    if (e instanceof ZodError) {
      return responseError(400, e.issues[0].message)
    }
  }
})
