import type { Database } from "firebase/database"

declare module "#app" {
  interface NuxtApp {
    rtdb: Database
  }
}

export {}
