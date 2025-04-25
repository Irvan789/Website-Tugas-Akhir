import moment from "moment"
import { ZodError } from "zod"

import { zDataValidation } from "~/utils/Validation"
import { IStatistics } from "~/types/database"

export default defineEventHandler(async (event) => {
  const data: IStatistics = await readBody(event)

  try {
    await zDataValidation.parseAsync(data)

    const timeNow = moment().unix()

    await statisticRef
      .child(`realtime`)
      .child(timeNow.toString())
      .update({
        vbatt: data.vbatt,
        vpln: data.vpln,
        vplts: data.vplts,
        lpln: data.lpln,
        lplts: data.lplts,
        relay: {
          n1: {
            load: data.relay.n1.load,
            source: data.relay.n1.source
          },
          n2: {
            load: data.relay.n2.load,
            source: data.relay.n2.source
          },
          n3: {
            load: data.relay.n3.load,
            source: data.relay.n3.source
          }
        },
        time: timeNow
      })

    const configTime = (await statisticRef.child("configTime").get()).val() as number

    return responseSuccess(event, 200, "OK", { configTime: configTime })
  } catch (e) {
    if (e instanceof ZodError) {
      return responseError(400, e.issues[0].message)
    }
  }
})
