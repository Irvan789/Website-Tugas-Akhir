import { getDatabase } from "firebase-admin/database"

const config = useRuntimeConfig()
const db = getDatabase()

const deviceId = config.public.deviceId

export const deviceRef = db.ref(`devices`).child(deviceId)
export const statisticRef = db.ref(`statistics`).child(deviceId)
