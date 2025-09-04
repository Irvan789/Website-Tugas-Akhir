import tailwindcss from "@tailwindcss/vite"

// https://nuxt.com/docs/api/configuration/nuxt-config
export default defineNuxtConfig({
  compatibilityDate: "2025-01-01",
  devtools: { enabled: true },
  app: {
    head: {
      charset: "utf-8",
      viewport:
        "width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover"
    }
  },
  css: ["~/assets/css/main.css"],
  modules: ["@nuxtjs/google-fonts"],
  googleFonts: {
    families: {
      "Signika Negative": true
    }
  },
  runtimeConfig: {
    apiKey: import.meta.env.APP_API_KEY,
    user: {
      name: import.meta.env.APP_USER_NAME,
      avatar: import.meta.env.APP_USER_AVATAR
    },
    firebase: {
      projectId: import.meta.env.FIREBASE_ADMIN_PROJECT_ID,
      privateKey: import.meta.env.FIREBASE_ADMIN_PRIVATE_KEY,
      clientEmail: import.meta.env.FIREBASE_ADMIN_CLIENT_EMAIL,
      databaseURL: import.meta.env.FIREBASE_ADMIN_DATABASE_URL
    },
    public: {
      openPublic: import.meta.env.APP_PUBLIC ?? "false",
      firebase: {
        apiKey: import.meta.env.FIREBASE_API_KEY,
        authDomain: import.meta.env.FIREBASE_AUTH_DOMAIN,
        projectId: import.meta.env.FIREBASE_PROJECT_ID,
        appId: import.meta.env.FIREBASE_APP_ID,
        databaseURL: import.meta.env.FIREBASE_DATABASE_URL
      },
      deviceId: import.meta.env.APP_DEVICE_ID
    }
  },
  vite: {
    plugins: [tailwindcss()]
  }
})
