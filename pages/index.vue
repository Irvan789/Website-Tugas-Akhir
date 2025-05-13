<script setup lang="ts">
import { ref as dbRef, limitToLast, onValue, query } from "firebase/database"

import moment from "moment"
import { Icon } from "@iconify/vue"

import type { IStatistics } from "~/types/database"

useSeoMeta({
  title: "Home · Tugas Akhir 119400018",
  ogTitle: "Home · Tugas Akhir 119400018",
  description: "Website Tugas Akhir Irvan Tonny Widjaya (119400018)",
  ogDescription: "Website Tugas Akhir Irvan Tonny Widjaya (119400018)"
})

const { $rtdb } = useNuxtApp()
const config = useRuntimeConfig()

const unixTime = ref<number>(unixTimeNow())

const statisticsLength = 15
const statisticsData = ref<IStatistics[]>([])
const statisticsDataLength = ref<number>(-1)

let unixInterval: NodeJS.Timeout

onMounted(() => {
  getRealTimeStatistic()

  unixInterval = setInterval(() => {
    unixTime.value = unixTimeNow()
  }, 1000)
})

onUnmounted(() => {
  clearInterval(unixInterval)
})

function getRealTimeStatistic() {
  let data: IStatistics[] = []
  onValue(
    query(
      dbRef($rtdb, `statistics/${config.public.deviceId}/realtime`),
      limitToLast(statisticsLength)
    ),
    (snapshot) => {
      data = []
      snapshot.forEach((v) => {
        data.push(v.val())
      })

      statisticsDataLength.value = data.length

      const unixTime = data[0]?.time ?? moment().unix()
      const statistics: IStatistics[] = []

      for (let i = 0; i < statisticsLength; i++) {
        const clockTime = unixTime - i

        if (data[i] != undefined) {
          statistics.push(data[i])
        } else {
          statistics.unshift({
            vpln: 0,
            vplts: 0,
            vbatt: 0.0,
            lpln: 0,
            lplts: 0,
            relay: {
              n1: { source: "plts", power: 0, current: 0.0 },
              n2: { source: "plts", power: 0, current: 0.0 },
              n3: { source: "plts", power: 0, current: 0.0 }
            },
            time: clockTime
          })
        }
      }

      statisticsData.value = statistics
    }
  )
}

function unixTimeNow() {
  return moment().unix()
}
</script>

<template>
  <div class="mx-auto grid w-full max-w-7xl grid-flow-row gap-y-2.5 px-3 py-3">
    <div
      v-if="statisticsDataLength != 0 && statisticsData.at(-1)?.time! + 10 <= unixTime"
      class="flex items-center gap-x-1.5 rounded border border-red-600 bg-red-500 px-2 py-1.5 text-[15px] text-white"
    >
      <Icon icon="mingcute:alert-fill" class="text-xl" />
      <span class="leading-5">
        Perangkat Terdeteksi Terakhir Aktif Pada
        <FormatTime :time="statisticsData.at(-1)?.time!" format="DD MMMM YYYY" /> Pukul
        <FormatTime :time="statisticsData.at(-1)?.time!" format="HH:mm:ss" />
      </span>
    </div>
    <Monitoring :statistics="statisticsData" />
  </div>
</template>
