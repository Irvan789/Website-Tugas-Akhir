import { H3Event } from "h3"

function responseSuccess(event: H3Event, code: number, message: string, data?: Object) {
  setResponseStatus(event)
  return {
    statusCode: code,
    message: message,
    data: data
  }
}

function responseError(code: number, message: string) {
  throw createError({
    statusCode: code,
    message: message
  })
}

export { responseError, responseSuccess }
