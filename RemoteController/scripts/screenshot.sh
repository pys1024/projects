#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_FILE="${ROOT_DIR}/ui/latest.png"

if ! command -v pio >/dev/null 2>&1; then
  echo "Error: pio not found in PATH" >&2
  exit 1
fi

if ! command -v xdotool >/dev/null 2>&1; then
  echo "Error: xdotool not found in PATH" >&2
  exit 1
fi

if ! command -v import >/dev/null 2>&1; then
  echo "Error: ImageMagick import not found in PATH" >&2
  exit 1
fi

if [[ -z "${DISPLAY:-}" ]]; then
  echo "Error: DISPLAY is not set" >&2
  exit 1
fi

mkdir -p "${ROOT_DIR}/ui"

runner_pid=""
window_pid=""

cleanup() {
  if [[ -n "${window_pid}" ]] && kill -0 "${window_pid}" >/dev/null 2>&1; then
    kill "${window_pid}" >/dev/null 2>&1 || true
    sleep 0.3
    kill -9 "${window_pid}" >/dev/null 2>&1 || true
  fi

  if [[ -n "${runner_pid}" ]] && kill -0 "${runner_pid}" >/dev/null 2>&1; then
    kill -TERM -"${runner_pid}" >/dev/null 2>&1 || true
    sleep 0.5
    kill -KILL -"${runner_pid}" >/dev/null 2>&1 || true
  fi
}

trap cleanup EXIT INT TERM

setsid bash -c "cd \"${ROOT_DIR}\" && pio run -e emulator_64bits -t execute" &
runner_pid="$!"

window_id=""
for _ in $(seq 1 100); do
  # Prefer visible windows and pick the most recently discovered one.
  for id in $(xdotool search --onlyvisible --name "LVGL Simulator" 2>/dev/null || true); do
    window_id="${id}"
  done

  if [[ -n "${window_id}" ]]; then
    break
  fi

  sleep 0.1
done

if [[ -z "${window_id}" ]]; then
  echo "Error: LVGL Simulator window not found" >&2
  exit 1
fi

window_pid="$(xdotool getwindowpid "${window_id}" 2>/dev/null || true)"

# Give the simulator a moment to draw and retry capture in case a stale window
# id was reported during startup/teardown.
sleep 2
captured=""
for _ in $(seq 1 20); do
  if import -window "${window_id}" "${OUT_FILE}" 2>/dev/null; then
    captured="yes"
    break
  fi

  # Re-resolve a visible window id between retries.
  for id in $(xdotool search --onlyvisible --name "LVGL Simulator" 2>/dev/null || true); do
    window_id="${id}"
  done

  sleep 0.2
done

if [[ -z "${captured}" ]]; then
  echo "Error: failed to capture LVGL Simulator window" >&2
  exit 1
fi

echo "Saved screenshot to ${OUT_FILE}"
