export default defineEventHandler(async (event) => {
  const runtime = useRuntimeConfig(event)

  if (
    runtime.public.openPublic == "true" &&
    event.path.startsWith("/api/config") &&
    event.method != "GET"
  ) {
    return responseError(400, "Invalid Request")
  }

  if (event.path.startsWith("/api/iot")) {
    const query: { key?: string; deviceId?: string } = getQuery(event)

    if (query.key != runtime.apiKey || query.deviceId != runtime.public.deviceId) {
      return responseError(401, "Unauthenticated")
    }

    return
  }

  return
})
