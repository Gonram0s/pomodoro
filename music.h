const int buzzer = A2;

void music () {
  tone(buzzer, 440, 500);
  delay(500); // Espera enquanto o tom está tocando

  // Segundo tom: 494 Hz por 500 ms
  tone(buzzer, 494, 500);
  delay(500);

  // Terceiro tom: 523 Hz por 500 ms
  tone(buzzer, 523, 500);
  delay(500);

  // Quarto tom: 587 Hz por 500 ms
  tone(buzzer, 587, 500);
  delay(500);

  // Quinto tom: 659 Hz por 500 ms
  tone(buzzer, 659, 500);
  delay(500);

  // Parar o som (opcional, pois o loop vai reiniciar e tocar de novo)
  noTone(buzzer);
}