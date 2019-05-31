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

<h1 class="contract">
   cnclrexorder
</h1>
---

spec-version: 0
title: Cancel REX order
summary: The `cnclrexorder` action cancels a queued REX sell order if one exists for an account.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to cancel any unfilled and queued REX sell orders that exist for the account {{ owner }}.

<h1 class="contract">
   consolidate
</h1>
---

spec-version: 0
title: Consolidate REX
summary: The `consolidate` action will consolidate all REX maturity buckets for an account into one that matures 4 days from 00:00 UTC.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to consolidate any open REX maturity buckets for the account {{ owner }} into one that matures 4 days from the following 00:00 UTC.

<h1 class="contract">
   defcpuloan
</h1>
---

spec-version: 0
title: Withdraw CPU loan
summary: The `defcpuloan` action allows an account to withdraw tokens from the fund of a specific CPU loan and adds them to REX fund.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to withdraw from the CPU loan fund identified by loan number {{ loan_num }} on the account {{ from }} in the amount of {{ amount }} and have those tokens allocated to the REX fund of {{ from }}.

<h1 class="contract">
   defnetloan
</h1>
---

spec-version: 0
title: Withdraw NET loan
summary: The `defnetloan` action allows an account to withdraw tokens from the fund of a specific Network loan and adds them to REX fund.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to withdraw from the Network loan fund identified by loan number {{ loan_num }} on the account {{ from }} in the amount of {{ amount }} and have those tokens allocated to the REX fund of {{ from }}.

<h1 class="contract">
   deposit
</h1>
---

spec-version: 0
title: Deposit EOS into REX
summary: The `deposit` action allows an account to deposit EOS tokens into REX fund by transfering from their liquid token balance.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to deposit {{ amount }} EOS tokens into the REX fund of the account {{ owner }} from the liquid token balance of {{ owner }}.

<h1 class="contract">
   fundcpuloan
</h1>
---

spec-version: 0
title: Fund CPU Loan
summary: The `fundcpuloan` action allows an account to transfer tokens from its REX fund to the fund of a specific CPU loan in order for those tokens to be used for loan renewal at the loan's expiry.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to transfer the amount of {{ payment }} tokens into the CPU loan fund of the loan identified by loan number {{ loan_num }} from the account {{ from }} to be used for loan renewal at the expiry of {{ loan_num }}.

<h1 class="contract">
   fundnetloan
</h1>
---

spec-version: 0
title: Fund NET Loan
summary: The `fundnetloan` action allows an account to transfer tokens from its REX fund to the fund of a specific Network loan in order for those tokens to be used for loan renewal at the loan's expiry.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to transfer the amount of {{ payment }} tokens into the Network loan fund of the loan identified by loan number {{ loan_num }} from the account {{ from }} to be used for loan renewal at the expiry of {{ loan_num }}.

<h1 class="contract">
   mvfrsavings
</h1>
---

spec-version: 0
title: Move REX from savings
summary: The `mvfrsavings` action allows an account to move REX tokens from its savings bucket to a bucket with a maturity date that is 4 days after 00:00 UTC.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to move {{ rex }} tokens from the savings bucket of the account {{ owner }}. Those tokens shall become available to {{ owner }} 4 days from 00:00 UTC.

<h1 class="contract">
   mvtosavings
</h1>
---

spec-version: 0
title: Move REX to savings
summary: The `mvtosavings` action allows an account to move REX tokens into a savings bucket.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to move {{ rex }} tokens to a savings bucket associated to the account {{ owner }}. I acknowledge that those tokens will then be subject to any maturity restrictions described in the `mvfrsavings` action.

<h1 class="contract">
   rentcpu
</h1>
---

spec-version: 0
title: Rent CPU
summary: The `rentcpu` action allows an account to rent CPU bandwidth for 30 days at a market-determined price.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to rent CPU bandwidth for 30 days for the use of the account {{ receiver }} in exchange for the loan payment of {{ loan_payment }}, which shall be taken from the account {{ from }}. The loan fund amount {{ loan_fund }} is set for automatic renewal of the loan at the expiry of said loan.

The amount of CPU bandwidth shall be determined by the market at time of loan execution and shall be recalculated at time of renewal, should I wish to automatically renew the loan at that time. I acknowledge that the amount of CPU bandwidth received in exchange of {{ loan_payment }} for the benefit of {{ receiver }} at loan renewal may be different from the current amount of bandwidth received.

<h1 class="contract">
   rentnet
</h1>
---

spec-version: 0
title: Rent NET
summary: The `rentnet` action allows an account to rent Network bandwidth for 30 days at a market-determined price.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to rent Network bandwidth for 30 days for the use of the account {{ receiver }} in exchange for the loan payment of {{ loan_payment }}, which shall be taken from the account {{ from }}. The loan fund amount {{ loan_fund }} is set for automatic renewal of the loan at the expiry of said loan.

The amount of Network bandwidth shall be determined by the market at time of loan execution and shall be recalculated at time of renewal, should I wish to automatically renew the loan at that time. I acknowledge that the amount of Network bandwidth received in exchange of {{ loan_payment }} for the benefit of {{ receiver }} at loan renewal may be different from the current amount of bandwidth received.

<h1 class="contract">
   rexexec
</h1>
---

spec-version: 0
title: REX Exec
summary: The `rexexec` action allows any account to perform REX maintenance by processing expired loans and unfilled sell orders.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

I, {{ signer }}, wish to process up to {{ max }} of any CPU loans, Network loans, and sell orders that may currently be pending.

<h1 class="contract">
   sellrex
</h1>
---

spec-version: 0
title: Sell REX
summary: The `sellrex` action allows an account to sell REX tokens held by the account.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to sell {{ rex }} REX tokens held on the account {{ from }} in exchange for core EOS tokens. If there is an insufficient amount of EOS tokens available at this time, I acknowledge that my order will be placed in a queue to be processed.

If there is an open `sellrex` order for the account {{ from }}, then this amount of {{ rex }} REX shall be added to the existing order and the order shall move to the back of the queue.

<h1 class="contract">
   unstaketorex
</h1>
---

spec-version: 0
title: Unstake to REX
summary: The `unstaketorex` action allows an account to buy REX using EOS tokens which are currently staked for either CPU or Network bandwidth.

icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to buy REX tokens by unstaking {{ from_cpu }} EOS from CPU bandwidth and {{ from_net }} EOS from Network bandwidth from account {{ owner }} that are staked to account {{ receiver }}.

I am aware of, and have fulfilled, all voting requirements needed to participate in the REX marketplace.

<h1 class="contract">
   withdraw
</h1>
---

spec-version: 0
title: withdraw from REX
summary: The `withdraw` action allows an account to withdraw EOS tokens from their REX fund into their liquid token balance.


icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I, {{ signer }}, wish to withdraw {{ amount }} of EOS tokens from the REX fund for the account {{ owner }} into its liquid token balance.
