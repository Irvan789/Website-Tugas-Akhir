import { FirebaseDatabaseError } from "firebase-admin/database"
import { responseError, responseSuccess } from "~/server/utils/ApiResponse"

export default defineEventHandler(async (event) => {
  try {
    const deviceConfig = await deviceRef.get()

    return responseSuccess(event, 200, "Berhasil Mengambil Data!", deviceConfig.val())
  } catch (e) {
    if (e instanceof FirebaseDatabaseError) {
      return responseError(400, e.message)
    }
  }
})
