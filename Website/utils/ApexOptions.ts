import moment from "moment"
import type { ApexOptions } from "apexcharts"

const acVoltage = 250
const dcVoltage = 25

const loadPower = 300

const options: ApexOptions = {
  noData: {
    text: "Tidak Ada Data!"
  },
  grid: {
    show: false
  }
}

const xaxis: ApexCharts.ApexOptions["xaxis"] = {
  type: "category",
  tickAmount: 15,
  title: {
    text: "Waktu (s)",
    offsetY: -6
  },
  labels: {
    formatter: (val) => {
      return moment.unix(parseInt(val)).format("ss")
    }
  }
}

export const chartVPLNOptions: ApexOptions = {
  title: {
    text: "Tegangan PLN",
    offsetX: -4,
    offsetY: 5
  },
  ...options,
  xaxis: xaxis,
  yaxis: {
    min: 0,
    max: acVoltage,
    title: {
      text: "Tegangan (V)"
    }
  }
}

export const chartVPLTSOptions: ApexOptions = {
  title: {
    text: "Tegangan PLTS",
    offsetX: -4,
    offsetY: 5
  },
  ...options,
  xaxis: xaxis,
  yaxis: {
    min: 0,
    max: acVoltage,
    title: {
      text: "Tegangan (V)"
    }
  }
}

export const chartVBATTOptions: ApexOptions = {
  title: {
    text: "Tegangan Baterai",
    offsetX: -4,
    offsetY: 5
  },
  ...options,
  xaxis: xaxis,
  yaxis: {
    min: 0,
    max: dcVoltage,
    title: {
      text: "Tegangan (V)"
    }
  }
}

export const chartLPLNOptions: ApexOptions = {
  title: {
    text: "Penggunaan Daya PLN",
    offsetX: -4,
    offsetY: 5
  },
  ...options,
  xaxis: xaxis,
  yaxis: {
    min: 0,
    max: loadPower,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    },
    title: {
      text: "Daya (W)"
    }
  }
}

export const chartLPLTSOptions: ApexOptions = {
  title: {
    text: "Penggunaan Daya PLTS",
    offsetX: -4,
    offsetY: 5
  },
  ...options,
  xaxis: xaxis,
  yaxis: {
    min: 0,
    max: loadPower,
    labels: {
      formatter: (val) => {
        return val.toFixed(0)
      }
    },
    title: {
      text: "Daya (W)"
    }
  }
}
