import moment from "moment"
import type { IDevices } from "~/types/database"

export const deviceInitConfig: IDevices = {
  automated: true,
  pln: {
    active: true
  },
  plts: {
    active: true,
    min_voltage: 220
  },
  relay: {
    n1: {
      source: "auto",
      max_load: 50
    },
    n2: {
      source: "auto",
      max_load: 70
    },
    n3: {
      source: "auto",
      max_load: 100
    }
  },
  configTime: moment().unix()
}
