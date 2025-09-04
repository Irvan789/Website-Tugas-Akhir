import moment from "moment"
import { ZodError } from "zod"

import { zDataValidation } from "~/utils/Validation"
import { IDevices, IStatistics } from "~/types/database"

export default defineEventHandler(async (event) => {
  const data: IStatistics = await readBody(event)

  try {
    await zDataValidation.parseAsync(data)

    const timeNow = moment().unix()

    await statisticRef
      .child("realtime")
      .child(timeNow.toString())
      .update({
        vbatt: data.vbatt,
        vpln: data.vpln,
        vplts: data.vplts,
        lpln: data.lpln,
        lplts: data.lplts,
        relay: {
          n1: {
            power: data.relay.n1.power,
            current: parseFloat(data.relay.n1.current.toFixed(3)),
            source: data.relay.n1.source
          },
          n2: {
            power: data.relay.n2.power,
            current: parseFloat(data.relay.n2.current.toFixed(3)),
            source: data.relay.n2.source
          },
          n3: {
            power: data.relay.n3.power,
            current: parseFloat(data.relay.n3.current.toFixed(3)),
            source: data.relay.n3.source
          }
        },
        time: timeNow
      })

    const deviceData = await deviceRef.get()
    const { configTime } = deviceData.val() as IDevices

    return responseSuccess(event, 200, "OK", { configTime: configTime })
  } catch (e) {
    if (e instanceof ZodError) {
      return responseError(400, e.issues[0].message)
    }
  }
})
