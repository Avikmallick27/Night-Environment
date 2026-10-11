# Night Environment

> An intelligent IoT-based telemetry and analytics platform for monitoring nocturnal environmental conditions to evaluate sleep quality factors and support exploratory health-screening prototypes.

---

## 📌 Project Overview

**Night Environment** is an end-to-end IoT research platform engineered to bridge room ambiance with sleep physiology. The system continuously logs nocturnal ambient metrics—such as ambient temperature, humidity, lighting levels, and air parameters—and synchronizes the telemetry to cloud storage in real time. Through a responsive live dashboard and historical time-series charts, users can correlate ambient room shifts with subjective sleep logs to identify environmental disruptions, optimize sleep hygiene, and provide structured baseline data for preventative wellness research.

> *Note: This project is an experimental engineering prototype designed for environmental tracking and sleep hygiene analysis. It is non-diagnostic and not intended for clinical use.*

---

## ✨ Features

- **Real-Time Telemetry:** Continuous multi-sensor acquisition of overnight environmental parameters.
- **Interactive Live Dashboard:** Clean, low-latency visual meters for immediate ambient status inspection.
- **Historical Trend Visualization:** Time-series charts to evaluate environmental fluctuations across days and weeks.
- **Sleep Record Management:** Structured manual logging interface for sleep duration, perceived quality, and disruptions.
- **Environmental Insights:** Comparative analysis matching room condition anomalies against reported sleep quality.
- **Cloud Synchronization:** Resilient automated cloud data forwarding for continuous remote logging.

---

## 📁 Repository Structure

```text
Night-Environment/
├── Firmware/          # Microcontroller source code, board pinouts, and sensor drivers
├── Web_Dashboard/     # Front-end interfaces, charting visualizers, and cloud config
├── Documentation/     # Circuit diagrams, hardware schematics, and setup notes
└── README.md          # Project overview and maintenance documentation
