## Build the C++ Client

### 1. Install dependencies

```bash
sudo apt install -y cmake libcurl4-openssl-dev nlohmann-json3-dev build-essential
```

---

### 2. Build

```bash
cd ~/sae2/client
mkdir build && cd build
cmake ..
make
```

---

### 3. Run

```bash
./sae2_client
```

It will ask you to fill in:
- **Hostname** — your machine name (e.g. `serverek`)
- **Website** — e.g. `example.com`
- **Login** — e.g. `admin`
- **Password** — min 8 characters

Then type `Send` to submit to the server.

---

### 4. Verify data was received

```bash
curl -k https://localhost/api/submissions
```

You should see your submitted data in the response.