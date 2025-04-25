import { getApp, getApps, initializeApp, type FirebaseOptions } from "firebase/app"
import { getDatabase } from "firebase/database"

export default defineNuxtPlugin(() => {
  const config = useRuntimeConfig()

  const firebaseConfig: FirebaseOptions = {
    apiKey: config.public.firebase.apiKey,
    authDomain: config.public.firebase.authDomain,
    projectId: config.public.firebase.projectId,
    appId: config.public.firebase.appId,
    databaseURL: config.public.firebase.databaseURL
  }

  const app = getApps().length > 0 ? getApp() : initializeApp(firebaseConfig)

  return {
    provide: {
      rtdb: getDatabase(app)
    }
  }
})