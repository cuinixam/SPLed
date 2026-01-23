# Software Architecture

## Components

All software components communicate exclusively through the **RTE (Runtime Environment)**, which provides standardized interfaces for data exchange. The architecture follows a layered approach:

```{mermaid}
flowchart TB
    subgraph ApplicationLayer["Application Layer (High-Level)"]
        PSP["PowerSignalProcessing"]
        LC["LightController"]
        BC["BrightnessController"]
    end

    subgraph RTE["RTE (Runtime Environment)"]
        direction LR
        subgraph Signals["Shared Signals"]
            PowerState["PowerState"]
            PowerKeyPressedEvent["PowerKeyPressedEvent"]
            MainKnobValue["MainKnobValue"]
            BrightnessValue["BrightnessValue"]
            LightValue["LightValue"]
            BrightnessAdjustmentCounter["BrightnessAdjustmentCounter"]
        end
    end

    subgraph PlatformLayer["Platform Layer (Low-Level)"]
        PB["PowerButton"]
        MK["MainControlKnob"]
        CI["ConsoleInterface"]
    end

    %% Application Layer to RTE connections
    PSP <--> RTE
    LC <--> RTE
    BC <--> RTE

    %% Platform Layer to RTE connections
    RTE <--> MK
    RTE <--> PB
    RTE <--> CI
```

### Layered Architecture

| Layer | Description |
|-------|-------------|
| **Application Layer** | High-level software components implementing business logic. All communication happens through the RTE. |
| **RTE** | Runtime Environment - middleware that provides standardized interfaces (signals) for inter-component communication. |
| **Platform Layer** | Low-level drivers and hardware abstraction. Platform-specific implementations that interface with hardware or OS. |

### Component Descriptions

| Component | Layer | Description |
|-----------|-------|-------------|
| **PowerSignalProcessing** | Application | Processes power button events and manages the power state |
| **LightController** | Application | Controls the light output based on power state, knob value, and brightness |
| **BrightnessController** | Application | Manages brightness adjustments based on the main control knob |
| **PowerButton** | Platform | Low-level driver for the power button input |
| **MainControlKnob** | Platform | Low-level driver for the main control knob input |
| **ConsoleInterface** | Platform | Low-level interface for displaying/outputting the light value |
