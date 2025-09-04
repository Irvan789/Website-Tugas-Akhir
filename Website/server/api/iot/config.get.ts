import { FirebaseDatabaseError } from "firebase-admin/database"
import { deviceInitConfig } from "~/utils/InitialData"

export default defineEventHandler(async (event) => {
  try {
    const deviceConfig = await deviceRef.get()

    return responseSuccess(event, 200, "OK", deviceConfig.val() ?? deviceInitConfig)
  } catch (e) {
    if (e instanceof FirebaseDatabaseError) {
      return responseError(400, e.message)
    }
  }
})
