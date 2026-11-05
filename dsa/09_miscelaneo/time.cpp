using Clock = chrono::steady_clock;
using TimePoint = Clock::time_point;
// Obtener instante actual
TimePoint getTime() { return Clock::now(); }

// Diferencia en segundos
double elapsedSeconds(TimePoint start) {
  return chrono::duration<double>(Clock::now() - start).count();
}

// Diferencia en milisegundos
ll elapsedMilliseconds(TimePoint start) {
  return chrono::duration_cast<chrono::milliseconds>(Clock::now() - start)
      .count();
}

// Diferencia en microsegundos
ll elapsedMicroseconds(TimePoint start) {
  return chrono::duration_cast<chrono::microseconds>(Clock::now() - start)
      .count();
}

// Diferencia en nanosegundos
ll elapsedNanoseconds(TimePoint start) {
  return chrono::duration_cast<chrono::nanoseconds>(Clock::now() - start)
      s.count();
}

// Dormir durante cierta cantidad de tiempo
void sleepSeconds(int s) { this_thread::sleep_for(chrono::seconds(s)); }

void sleepMilliseconds(int ms) {
  this_thread::sleep_for(chrono::milliseconds(ms));
}