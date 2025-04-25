export interface IDevices {
  automated: boolean
  pln: {
    active: boolean
  }
  plts: {
    active: boolean
    min_voltage: number
  }
  battery?: {
    min_voltage: number
  }
  relay: {
    n1: {
      source: string
      max_load: number
    }
    n2: {
      source: string
      max_load: number
    }
    n3: {
      source: string
      max_load: number
    }
  }
  configTime?: number
}

export interface IStatistics {
  vpln: number
  vplts: number
  vbatt: number
  lpln: number
  lplts: number
  relay: {
    n1: {
      load: number
      source: string
    }
    n2: {
      load: number
      source: string
    }
    n3: {
      load: number
      source: string
    }
  }
  time: number
}
