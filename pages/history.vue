<script setup lang="ts">
import { toast } from "vue3-toastify"

import { ref as dbRef, query, startAt, endAt, get, orderByChild } from "firebase/database"
import { FirebaseError } from "firebase/app"

import moment from "moment"

import type { IStatistics } from "~/types/database"
import { chartVBATTOptions } from "~/utils/ApexOptions"

useSeoMeta({
  title: "Histori · Tugas Akhir 119400018",
  ogTitle: "Histori · Tugas Akhir 119400018",
  description: "Website Tugas Akhir Irvan Tonny Widjaya (119400018)",
  ogDescription: "Website Tugas Akhir Irvan Tonny Widjaya (119400018)"
})

const { $rtdb } = useNuxtApp()
const config = useRuntimeConfig()

const unixTime = moment().set("second", 0).unix()
const statisticsData = ref<Array<IStatistics>>([])

const dateTime = reactive({
  start: unixTime - 600,
  end: unixTime
})

function getDataFromTime() {
  try {
    if (dateTime.start == dateTime.end) {
      throw Error("Periode Awal Tidak Boleh Sama Dengan Periode Akhir!")
    }

    if (dateTime.end < dateTime.start) {
      throw Error("Periode Akhir Tidak Boleh Kurang Dari Periode Awal!")
    }

    if (dateTime.end - dateTime.start > 600) {
      throw Error("Rentang Periode Tidak Boleh Lebih Dari 10 Menit!")
    }

    get(
      query(
        dbRef($rtdb, `statistics/${config.public.deviceId}/realtime`),
        orderByChild("time"),
        startAt(dateTime.start),
        endAt(dateTime.end)
      )
    )
      .then((val) => {
        const record = val.toJSON() as { [key: string]: Object } | null

        if (record) {
          const keyTime = Object.keys(record)
          statisticsData.value = keyTime.map((v) => record[v]) as Array<IStatistics>
        } else {
          statisticsData.value = []
        }
      })
      .catch((e) => {
        if (e instanceof FirebaseError) {
          throw Error(e.message)
        }
      })
  } catch (e) {
    if (e instanceof Error) {
      toast.error(e.message, toastOptions)
    }
  }
}

function handleDateChange(v: string, time: "start" | "end") {
  dateTime[time == "start" ? "start" : "end"] = moment(v).unix()
}

function dateTimeFormat(time: number) {
  return moment.unix(time).format("YYYY-MM-DDTHH:mm")
}
</script>

<template>
  <CardBlock class="flex flex-col gap-2.5 px-3 py-2.5 sm:flex-row sm:items-center">
    <span class="leading-4">Periode:</span>
    <div class="flex w-full flex-col justify-between gap-2.5 sm:flex-row">
      <div class="flex flex-row gap-2.5">
        <InputDateTime
          :value="dateTimeFormat(dateTime.start)"
          @change="(v) => handleDateChange(v, 'start')"
        />
        -
        <InputDateTime
          :value="dateTimeFormat(dateTime.end)"
          @change="(v) => handleDateChange(v, 'end')"
        />
      </div>
      <ButtonPrimary type="button" class="h-fit w-fit px-2.5 py-1 text-sm" @click="getDataFromTime">
        Cari Data
      </ButtonPrimary>
    </div>
  </CardBlock>
  <ClientOnly>
    <CardGrap>
      <ApexChart
        height="280"
        :options="chartVPLNOptions"
        :series="[
          {
            name: 'Tegangan PLN',
            data: statisticsData.map((v) => ({
              x: v.time.toString(),
              y: v.vpln
            }))
          }
        ]"
      />
    </CardGrap>
    <CardGrap>
      <ApexChart
        height="280"
        :options="chartVPLTSOptions"
        :series="[
          {
            name: 'Tegangan PLTS',
            data: statisticsData.map((v) => ({
              x: v.time.toString(),
              y: v.vplts
            }))
          }
        ]"
      />
    </CardGrap>
    <CardGrap>
      <ApexChart
        height="280"
        :options="chartVBATTOptions"
        :series="[
          {
            name: 'Tegangan Baterai',
            data: statisticsData.map((v) => ({
              x: v.time.toString(),
              y: v.vbatt
            }))
          }
        ]"
      />
    </CardGrap>
    <CardGrap>
      <ApexChart
        height="280"
        :options="chartLPLNOptions"
        :series="[
          {
            name: 'Penggunaan Daya PLN',
            data: statisticsData.map((v) => ({
              x: v.time.toString(),
              y: v.lpln
            }))
          }
        ]"
      />
    </CardGrap>
    <CardGrap>
      <ApexChart
        height="280"
        :options="chartLPLTSOptions"
        :series="[
          {
            name: 'Penggunaan Daya PLTS',
            data: statisticsData.map((v) => ({
              x: v.time.toString(),
              y: v.lplts
            }))
          }
        ]"
      />
    </CardGrap>
  </ClientOnly>
</template>
