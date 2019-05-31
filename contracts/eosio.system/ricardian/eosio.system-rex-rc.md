<h1 class="contract">
   buyrex
</h1>
---
spec-version: 0
title: Buy REX
summary:
  The `buyrex` action allows an account to buy REX in exchange for tokens taken
  out of the user's REX fund.
icon:
  https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to buy REX on the account {{ from }} with the amount {{ amount }} EOS. ￼I am aware of, and have fulfilled, all voting requirements needed to participate in the REX marketplace. ￼￼

<h1 class="contract">
   closerex
</h1>
---

spec-version: 0
title: Close REX
summary: The `closerex` action allows an account to delete unused REX-related database entries and frees occupied RAM associated with its storage.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party, I {{ signer }}, wish to delete all unused REX-related database entries from the account {{ owner }}.

I will not be able to successfully call `closerex` unless all checks for CPU loans, NET loans or refunds pending refunds are still processing on the account {{ owner }}.
