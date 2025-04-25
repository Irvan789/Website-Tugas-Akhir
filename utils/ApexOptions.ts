import type { ApexOptions } from "apexcharts"

const maxVoltage = 250
const maxPower = 500

export const chartVPLNOptions: ApexOptions = {
  yaxis: {
    min: 0,
    max: maxVoltage
  },
  title: {
    text: "Tegangan PLN (V)"
  }
}

export const chartVPLTSOptions: ApexOptions = {
  yaxis: {
    min: 0,
    max: maxVoltage
  },
  title: {
    text: "Tegangan PLTS (V)"
  }
}

export const chartLPLNOptions: ApexOptions = {
  yaxis: {
    min: 0,
    max: maxPower,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    }
  },
  title: {
    text: "Penggunaan Daya PLN (W)"
  }
}

export const chartLPLTSOptions: ApexOptions = {
  yaxis: {
    min: 0,
    max: maxPower,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    }
  },
  title: {
    text: "Penggunaan Daya PLTS (W)"
  }
}
