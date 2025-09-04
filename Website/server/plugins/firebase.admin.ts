import { cert, getApps, initializeApp } from "firebase-admin/app"

export default defineNitroPlugin(() => {
  const config = useRuntimeConfig()

  const firebaseAdminConfig = {
    credential: cert({
      projectId: config.firebase.projectId,
      privateKey: config.firebase.privateKey.replace(/\\n/g, '\n'),
      clientEmail: config.firebase.clientEmail
    }),
    databaseURL: config.firebase.databaseURL
  }

  if (getApps().length < 1) {
    initializeApp(firebaseAdminConfig)
  }
})