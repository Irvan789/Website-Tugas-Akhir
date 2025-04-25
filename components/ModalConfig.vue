<script setup lang="ts">
import { toast } from "vue3-toastify"

import { FetchError } from "ofetch"
import { ZodError } from "zod"

import { Icon } from "@iconify/vue"

import type { IDevices } from "~/types/database"
import type { IoTDeviceConfigAPI } from "~/types/api"

const props = defineProps<{
  display: boolean
}>()

const emits = defineEmits<{
  close: []
}>()

const { data: res } = await useFetch<IoTDeviceConfigAPI>("/api/config")

const btnRef = useTemplateRef("btnRef")
const initCfg = ref<boolean>(!!res.value?.data)

const config = reactive<IDevices>({
  automated: res.value?.data?.automated ?? deviceInitConfig.automated,
  pln: {
    active: res.value?.data?.pln.active ?? deviceInitConfig.pln.active
  },
  plts: {
    active: res.value?.data?.plts.active ?? deviceInitConfig.plts.active,
    min_voltage: res.value?.data?.plts.min_voltage ?? deviceInitConfig.plts.min_voltage
  },
  relay: {
    n1: {
      source: res.value?.data?.relay.n1.source ?? deviceInitConfig.relay.n1.source,
      max_load: res.value?.data?.relay.n1.max_load ?? deviceInitConfig.relay.n1.max_load
    },
    n2: {
      source: res.value?.data?.relay.n2.source ?? deviceInitConfig.relay.n2.source,
      max_load: res.value?.data?.relay.n2.max_load ?? deviceInitConfig.relay.n2.max_load
    },
    n3: {
      source: res.value?.data?.relay.n3.source ?? deviceInitConfig.relay.n3.source,
      max_load: res.value?.data?.relay.n3.max_load ?? deviceInitConfig.relay.n3.max_load
    }
  }
})

watch(config, () => {
  if (config.automated == true) {
    config.pln.active = true
    config.plts.active = true
    config.relay.n1.source = "auto"
    config.relay.n2.source = "auto"
    config.relay.n3.source = "auto"
  }

  if (config.relay.n1.source == "pln") {
    config.relay.n1.max_load = 0
  }

  if (config.relay.n2.source == "pln") {
    config.relay.n2.max_load = 0
  }

  if (config.relay.n3.source == "pln") {
    config.relay.n3.max_load = 0
  }
})

async function initConfig() {
  const button = btnRef.value?.componentRef as HTMLButtonElement
  button.setAttribute("disabled", "disabled")

  try {
    const res = await $fetch("/api/config", {
      method: "PUT"
    })

    if (res?.statusCode == 200) {
      toast.success(`${res.message}!`, toastOptions)
      initCfg.value = true
    }
  } catch (e) {
    if (e instanceof Error) {
      toast.error(`${e.message}!`, toastOptions)
    } else {
      console.log(e)
    }
  } finally {
    button.removeAttribute("disabled")
  }
}

async function saveConfig() {
  const button = btnRef.value?.componentRef as HTMLButtonElement

  try {
    button.setAttribute("disabled", "disabled")

    await zConfigValidation.parseAsync(config)

    const res = await $fetch("/api/config", {
      method: "PATCH",
      body: config
    })

    toast.success(res.message, toastOptions)
  } catch (e) {
    const errorMessage =
      e instanceof ZodError
        ? e.issues[0].message
        : e instanceof FetchError
          ? e.data.message
          : e instanceof Error
            ? e.message
            : e

    toast.error(errorMessage, toastOptions)
  } finally {
    button.removeAttribute("disabled")
  }
}
</script>

<template>
  <div
    class="absolute top-0 left-0 z-[100] h-full w-full bg-white duration-300"
    :class="[
      props.display
        ? 'visible opacity-100 transition-opacity'
        : 'invisible opacity-0 transition-all'
    ]"
  >
    <div class="flex min-h-full w-full flex-col px-3 py-3">
      <div
        class="m-auto flex w-full max-w-[450px] flex-col gap-y-5 rounded-lg bg-[#d0ebfe] px-5 py-5 text-[#19335a]"
      >
        <div class="relative mb-1 flex items-center justify-center text-center text-xl font-bold">
          <span class="leading-5">Konfigurasi Perangkat</span>
          <button class="absolute right-0 cursor-pointer" @click="emits('close')">
            <Icon icon="mingcute:close-fill" />
          </button>
        </div>
        <form v-if="initCfg" class="flex flex-col gap-y-5" @submit.prevent="saveConfig">
          <div class="relative rounded border-2 border-sky-200 px-2 py-2.5">
            <div class="absolute -top-[9px] left-1 bg-[#d0ebfe] px-1 text-xs font-semibold">
              Kontrol Sumber Listrik
            </div>
            <div class="grid grid-flow-row gap-1.5">
              <InputToggle
                name="Kontrol Secara Otomatis"
                :value="config.automated"
                @change="(e) => (config.automated = e)"
              />
              <InputToggle
                name="Sumber Listrik PLN"
                :value="config.pln.active"
                :disabled="config.automated"
                @change="(e) => (config.pln.active = e)"
              />
              <InputToggle
                name="Sumber Listrik PLTS"
                :value="config.plts.active"
                :disabled="config.automated"
                @change="(e) => (config.plts.active = e)"
              />
            </div>
          </div>
          <div class="relative rounded border-2 border-sky-200 px-2 py-2.5">
            <div class="absolute -top-[9px] left-1 bg-[#d0ebfe] px-1 text-xs font-semibold">
              Tegangan Minimal (Source)
            </div>
            <InputNumber
              name="Tegangan Minimal PLTS"
              unit="V"
              :value="config.plts.min_voltage"
              :step="1"
              @change="(e) => (config.plts.min_voltage = e)"
            />
          </div>
          <div class="relative rounded border-2 border-sky-200 px-2 py-2.5">
            <div class="absolute -top-[9px] left-1 bg-[#d0ebfe] px-1 text-xs font-semibold">
              Sumber Listrik (Source)
            </div>
            <div class="grid grid-flow-row gap-1.5">
              <InputSelect
                name="Sumber Listrik Relay N1"
                :value="['auto', 'pln', 'plts']"
                :selected="config.relay.n1.source"
                :disabled="config.automated"
                @change="(e) => (config.relay.n1.source = e)"
              />
              <InputSelect
                name="Sumber Listrik Relay N2"
                :value="['auto', 'pln', 'plts']"
                :selected="config.relay.n2.source"
                :disabled="config.automated"
                @change="(e) => (config.relay.n2.source = e)"
              />
              <InputSelect
                name="Sumber Listrik Relay N3"
                :value="['auto', 'pln', 'plts']"
                :selected="config.relay.n3.source"
                :disabled="config.automated"
                @change="(e) => (config.relay.n3.source = e)"
              />
            </div>
          </div>
          <div class="relative rounded border-2 border-sky-200 px-2 py-2.5">
            <div class="absolute -top-[9px] left-1 bg-[#d0ebfe] px-1 text-xs font-semibold">
              Maksimal Beban (Load)
            </div>
            <div class="grid grid-flow-row gap-1.5">
              <InputNumber
                name="Maksimal Beban Relay N1"
                unit="W"
                :value="config.relay.n1.max_load"
                :step="1"
                :disabled="config.relay.n1.source == 'pln' || !config.plts"
                @change="(e) => (config.relay.n1.max_load = e)"
              />
              <InputNumber
                name="Maksimal Beban Relay N2"
                unit="W"
                :value="config.relay.n2.max_load"
                :step="1"
                :disabled="config.relay.n2.source == 'pln' || !config.plts"
                @change="(e) => (config.relay.n2.max_load = e)"
              />
              <InputNumber
                name="Maksimal Beban Relay N3"
                unit="W"
                :value="config.relay.n3.max_load"
                :step="1"
                :disabled="config.relay.n3.source == 'pln' || !config.plts"
                @change="(e) => (config.relay.n3.max_load = e)"
              />
            </div>
          </div>
          <ButtonPrimary
            type="submit"
            class="bg-[#20529d]/90 text-white hover:bg-[#20529d] disabled:bg-[#2688f0]"
            ref="btnRef"
          >
            Simpan Konfigurasi
          </ButtonPrimary>
        </form>
        <form v-else class="flex flex-col gap-y-5" @submit.prevent="initConfig">
          <div
            class="rounded bg-yellow-600 px-2 py-2 text-sm leading-[18px] font-medium text-white"
          >
            Tidak Ada Konfigurasi Yang Tersimpan, Anda Harus Menginisiasi Konfigurasi Terlebih
            Dahulu!
          </div>
          <ButtonPrimary
            type="submit"
            class="bg-[#20529d]/90 text-white hover:bg-[#20529d] disabled:bg-[#2688f0]"
            ref="btnRef"
          >
            Inisiasi Konfigurasi
          </ButtonPrimary>
        </form>
      </div>
    </div>
  </div>
</template>
