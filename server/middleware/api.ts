export default defineEventHandler(async (event) => {
  const runtime = useRuntimeConfig()

  if (event.path.startsWith("/api/iot")) {
    const query: { key?: string; deviceId?: string } = getQuery(event)

    if (query.key != runtime.apiKey || query.deviceId != runtime.public.deviceId) {
      return responseError(401, "Unauthenticated")
    }

    return
  }

  return
})
