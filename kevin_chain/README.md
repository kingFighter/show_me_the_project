# KevinChain
Learn blockchain & Python by building one
## Concept
- [ ] HTTP interface to control the nodes
- [ ] Manabed by p2p
  - [ ] Protocol for inter-node communication
  - [ ] Validating new blocks
- [ ] Choosing the logest chain
- [ ] Proof-Of-Work
- [ ] Transaction
- [ ] Wallets


## Design
### Block structure
According to [Wiki](https://en.wikipedia.org/wiki/Blockchain)
- previousHash : a cryptographic hash of the previous block
- timestamp: block created time
- data: any data

### Protocol
Whenever a peer receives a higher-scoring version (usually the old version with a single new block added) they extend or overwrite their own database and retransmit the improvement to their peers.

