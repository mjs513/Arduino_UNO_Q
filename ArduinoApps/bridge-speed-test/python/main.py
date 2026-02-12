import time
import math
from arduino.app_utils import *

def run_raw_latency_test():
    """
    Measures the raw round-trip latency of a call to the Arduino.
    """
    print("--- Running RAW Latency Test ---")
    time.sleep(2)
    
    num_executions = 10
    latencies = []

    print(f"  Performing {num_executions} test calls...")

    for i in range(num_executions):
        try:
            start_time = time.perf_counter()
            Bridge.call("ping")
            end_time = time.perf_counter()
            
            duration_ms = (end_time - start_time) * 1000
            latencies.append(duration_ms)
            time.sleep(0.1)
        except Exception as e:
            print(f"  Error during ping execution {i+1}: {e}")
            continue

    if not latencies:
        print("  Raw Latency test failed: No successful pings.")
        print("----------------------------")
        return

    min_latency = min(latencies)
    max_latency = max(latencies)
    avg_latency = sum(latencies) / len(latencies)

    print("\n  --- Raw Latency Results ---")
    print(f"  Min Latency: {min_latency:.2f} ms")
    print(f"  Max Latency: {max_latency:.2f} ms")
    print(f"  Avg Latency: {avg_latency:.2f} ms")
    print("----------------------------")
    time.sleep(1)

def loop():
  # Run the latency tests first.
  run_raw_latency_test()
  time.sleep(0.1)

# Then, start the main application loop.
App.run(user_loop=loop)