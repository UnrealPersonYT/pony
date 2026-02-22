<p align="center">
  <h1 align="center">🐎 Pony — an Experimental Asymmetric ARX Stream Cipher</h1>
</p>

<p align="center">
  Pony is a tiny ARX-based stream cipher built around a compact, asymmetric round function.<br>
  It is not intended for production use — just exploration, experimentation, and fun.
</p>

---

<p align="center">
  <h2 align="center">✨ Core Idea</h2>
</p>

<p align="center">
  The heart of Pony is <code>parxw</code>, a 4‑word ARX step with two independent lanes:<br>
  an add/rotate lane and an xor lane.<br><br>
  These lanes do <em>not</em> depend on each other inside a round,<br>
  but they <em>do</em> feed each other’s inputs in the next round.<br><br>
  This creates a two‑round mixing cycle where the lanes:<br>
  <strong>diverge → cross → converge → diverge → …</strong>
</p>

---

<p align="center">
  <h2 align="center">🔀 Asymmetric by Design</h2>
</p>

<p align="center">
  Unlike symmetric ARX designs (e.g., ChaCha20), Pony’s round is directional.<br>
  The update path <code>b → a → d → c → b</code> is intentionally lopsided.<br><br>
  This asymmetry produces fast, irregular diffusion and short dependency chains,<br>
  giving Pony its “snappy” and chaotic behavior.
</p>

---

<p align="center">
  <h2 align="center">🧪 Experimental</h2>
</p>

<p align="center">
  Pony is a concept cipher — a playground for ARX ideas.<br>
  You are encouraged to experiment with:
</p>

<p align="center">
  • the key schedule<br>
  • IV handling<br>
  • rotation constants<br>
  • number of rounds<br>
  • state layout<br>
</p>

<p align="center">
  The core idea lives in <code>parxw</code>.
</p>

---

<p align="center">
  <h2 align="center">📦 What This Repository Contains</h2>
</p>

<p align="center">
  • Reference implementation<br>
  • Key schedule<br>
  • 16‑round block function<br>
  • Simple Pony‑CTR stream cipher<br>
</p>

---

<p align="center">
  <h2 align="center">🎯 Purpose</h2>
</p>

<p align="center">
  Pony explores how far a minimal asymmetric ARX engine can go<br>
  in terms of diffusion, structure, and performance.<br><br>
  It is not meant to be secure — just interesting.
</p>

---

<p align="center">
  <h2 align="center">🐎 Have fun.</h2>
</p>
