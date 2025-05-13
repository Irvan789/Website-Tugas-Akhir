import type { ApexOptions } from "apexcharts"
import moment from "moment"

const voltage = 250
const power = 250

const xaxis: ApexOptions = {
  xaxis: {
    type: "category",
    tickAmount: 15,
    labels: {
      formatter: (val) => {
        return unixFormat(parseInt(val), "ss")
      }
    }
  }
}

export const chartVPLNOptions: ApexOptions = {
  ...xaxis,
  yaxis: {
    min: 0,
    max: voltage
  },
  title: {
    text: "Tegangan PLN (V)",
    offsetX: 6,
    offsetY: 5
  }
}

export const chartVPLTSOptions: ApexOptions = {
  ...xaxis,
  yaxis: {
    min: 0,
    max: voltage
  },
  title: {
    text: "Tegangan PLTS (V)",
    offsetX: 6,
    offsetY: 5
  }
}

export const chartLPLNOptions: ApexOptions = {
  ...xaxis,
  yaxis: {
    min: 0,
    max: power,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    }
  },
  title: {
    text: "Penggunaan Daya PLN (W)",
    offsetX: 6,
    offsetY: 5
  }
}

export const chartLPLTSOptions: ApexOptions = {
  ...xaxis,
  yaxis: {
    min: 0,
    max: power,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    }
  },
  title: {
    text: "Penggunaan Daya PLTS (W)",
    offsetX: 6,
    offsetY: 5
  }
}

function unixFormat(time: number, format: string) {
  return moment.unix(time).format(format)
}
