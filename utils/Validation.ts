import * as z from "zod"

export const zConfigValidation = z
  .object({
    automated: z.boolean({
      required_error: "Automated Tidak Boleh Kosong!",
      invalid_type_error: "Input Type Automated Tidak Valid!"
    }),
    pln: z.object({
      active: z.boolean({
        required_error: "Sumber PLN Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Sumber PLN Tidak Valid!"
      })
    }),
    plts: z.object({
      active: z.boolean({
        required_error: "Sumber PLTS Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Sumber PLTS Tidak Valid!"
      }),
      min_voltage: z.number({
        required_error: "Minimal Tegangan PLTS Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Minimal Tegangan PLTS Tidak Valid!"
      })
    }),
    relay: z.object({
      n1: z.object({
        source: z.enum(["auto", "pln", "plts"], {
          required_error: "Sumber Listrik Relay N1 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Sumber Listrik Relay N1 Tidak Valid!"
        }),
        max_load: z.number({
          required_error: "Maksimal Load Relay N1 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Maksimal Load Relay N1 Tidak Valid!"
        })
      }),
      n2: z.object({
        source: z.enum(["auto", "pln", "plts"], {
          required_error: "Sumber Listrik Relay N2 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Sumber Listrik Relay N2 Tidak Valid!"
        }),
        max_load: z.number({
          required_error: "Maksimal Load Relay N2 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Maksimal Load Relay N2 Tidak Valid!"
        })
      }),
      n3: z.object({
        source: z.enum(["auto", "pln", "plts"], {
          required_error: "Sumber Listrik Relay N3 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Sumber Listrik Relay N3 Tidak Valid!"
        }),
        max_load: z.number({
          required_error: "Maksimal Load Relay N3 Tidak Boleh Kosong!",
          invalid_type_error: "Input Type Maksimal Load Relay N3 Tidak Valid!"
        })
      })
    })
  })
  .superRefine((val, ctx) => {
    if (val.automated) {
      if (!val.pln || !val.plts) {
        ctx.addIssue({
          code: z.ZodIssueCode.custom,
          message: "Saat Fitur Automated Diaktifkan, Sumber PLN dan Sumber PLTS Harus Dinyalakan!"
        })
      }

      if (
        val.relay.n1.source != "auto" ||
        val.relay.n2.source != "auto" ||
        val.relay.n3.source != "auto"
      ) {
        ctx.addIssue({
          code: z.ZodIssueCode.custom,
          message: "Saat Fitur Automated Diaktifkan, Sumber Listrik Harus Diposisi Auto!"
        })
      }
    }

    if (!val.pln) {
      if (
        val.relay.n1.source == "pln" ||
        val.relay.n2.source == "pln" ||
        val.relay.n3.source == "pln"
      ) {
        ctx.addIssue({
          code: z.ZodIssueCode.custom,
          message: "Sumber Listrik PLN Tidak Diaktifkan, Tidak Bisa Menggunakan Sumber Listrik!"
        })
      }
    }

    if (!val.plts) {
      if (
        val.relay.n1.source == "plts" ||
        val.relay.n2.source == "plts" ||
        val.relay.n3.source == "plts"
      ) {
        ctx.addIssue({
          code: z.ZodIssueCode.custom,
          message: "Sumber Listrik PLTS Tidak Diaktifkan, Tidak Bisa Menggunakan Sumber Listrik!"
        })
      }
    }

    const maxLoad = 220

    if (val.relay.n1.max_load + val.relay.n2.max_load + val.relay.n3.max_load > maxLoad) {
      ctx.addIssue({
        code: z.ZodIssueCode.custom,
        message: `Total Maksimal Beban (Load) Tidak Boleh Lebih Dari ${maxLoad} Watt!`
      })
    }
  })

export const zDataValidation = z.object({
  vbatt: z.number({
    required_error: "Tegangan Baterai Tidak Boleh Kosong!",
    invalid_type_error: "Input Type Tegangan Baterai Tidak Valid!"
  }),
  vpln: z.number({
    required_error: "Tegangan PLN Tidak Boleh Kosong!",
    invalid_type_error: "Input Type Tegangan PLN Tidak Valid!"
  }),
  vplts: z.number({
    required_error: "Tegangan PLTS Tidak Boleh Kosong!",
    invalid_type_error: "Input Type Tegangan PLTS Tidak Valid!"
  }),
  lpln: z.number({
    required_error: "Total Beban PLN Tidak Boleh Kosong!",
    invalid_type_error: "Input Type Total Beban PLN Tidak Valid!"
  }),
  lplts: z.number({
    required_error: "Total Beban PLTS Tidak Boleh Kosong!",
    invalid_type_error: "Input Type Total Beban PLTS Tidak Valid!"
  }),
  relay: z.object({
    n1: z.object({
      source: z.enum(["pln", "plts"], {
        required_error: "Sumber Listrik Relay N1 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Sumber Listrik Relay N1 Tidak Valid!"
      }),
      load: z.number({
        required_error: "Load Relay N1 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Load Relay N1 Tidak Valid!"
      })
    }),
    n2: z.object({
      source: z.enum(["pln", "plts"], {
        required_error: "Sumber Listrik Relay N2 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Sumber Listrik Relay N2 Tidak Valid!"
      }),
      load: z.number({
        required_error: "Load Relay N2 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Load Relay N2 Tidak Valid!"
      })
    }),
    n3: z.object({
      source: z.enum(["pln", "plts"], {
        required_error: "Sumber Listrik Relay N3 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Sumber Listrik Relay N3 Tidak Valid!"
      }),
      load: z.number({
        required_error: "Load Relay N3 Tidak Boleh Kosong!",
        invalid_type_error: "Input Type Load Relay N3 Tidak Valid!"
      })
    })
  })
})
