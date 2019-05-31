<h1 class="contract">
   bidname
</h1>
---
spec-version: 0.0
title: Bid on premium account name
summary: The {{ bidname }} action places a bid on a premium account name, in the knowledge that the high bid will purchase the name.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to bid on behalf of {{ bidder }} the amount of {{ bid }} toward purchase of the account name {{ newname }}.

<h1 class="contract">
   buyram
</h1>
---
spec-version: 0.0
title: Buy RAM
summary: This action will attempt to reserve about {{quant}} worth of RAM on behalf of {{receiver}}.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

{{buyer}} authorizes this contract to transfer {{quant}} to buy RAM based upon the current price as determined by the market maker algorithm.

{{buyer}} accepts that a 0.5% fee will be charged on the amount spent and that the actual RAM received may be slightly less than expected due to the approximations necessary to enable this service. {{buyer}} accepts that a 0.5% fee will be charged if and when they sell the RAM received. {{buyer}} accepts that rounding errors resulting from limits of computational precision may result in less RAM being allocated. {{buyer}} acknowledges that the supply of RAM may be increased at any time up to the limits of off-the-shelf computer equipment and that this may result in RAM selling for less than purchase price. {{buyer}} acknowledges that the price of RAM may increase or decrease over time according to supply and demand. {{buyer}} acknowledges that RAM is non-transferrable. {{buyer}} acknowledges RAM currently in use by their account cannot be sold until it is freed and that freeing RAM may be subject to terms of other contracts.

<h1 class="contract">
   buyrambytes
</h1>
---
spec-version: 0.0
title: Buy RAM Bytes
summary: This action will attempt to reserve about {{bytes}} bytes of RAM on behalf of {{receiver}}.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

{{buyer}} authorizes this contract to transfer sufficient EOS tokens to buy the RAM based upon the current price as determined by the market maker algorithm.

{{buyer}} accepts that a 0.5% fee will be charged on the EOS spent and that the actual RAM received may be slightly less than requested due to the approximations necessary to enable this service. {{buyer}} accepts that a 0.5% fee will be charged if and when they sell the RAM received. {{buyer}} accepts that rounding errors resulting from limits of computational precision may result in less RAM being allocated. {{buyer}} acknowledges that the supply of RAM may be increased at any time up to the limits of off-the-shelf computer equipment and that this may result in RAM selling for less than purchase price. {{buyer}} acknowledges that the price of RAM may increase or decrease over time according to supply and demand. {{buyer}} acknowledges that RAM is non-transferable. {{buyer}} acknowledges RAM currently in use by their account cannot be sold until it is freed and that freeing RAM may be subject to terms of other contracts.

<h1 class="contract">
   canceldelay
</h1>
---
spec-version: 0.0
title: Cancel delayed transaction
summary: The {{ canceldelay }} action cancels an existing delayed transaction.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to invoke the authority of {{ canceling_auth }} to cancel the transaction with ID {{ trx_id }}.

<h1 class="contract">
   claimrewards
</h1>
---
spec-version: 0.0
title: Claim rewards
summary: The {{ claimrewards }} action allows a block producer (active or standby) to claim the system rewards due them for producing blocks and receiving votes.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to have the rewards earned by {{ owner }} deposited into the {{ owner }} account.

<h1 class="contract">
   delegatebw
</h1>
---
spec-version: 0.0
title: Delegate Bandwidth
summary: The intent of the {{ delegatebw }} action is to stake tokens for bandwidth and/or CPU and optionally transfer ownership.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to stake {{ stake_cpu_quantity }} for CPU and {{ stake_net_quantity }} for bandwidth from the liquid tokens of {{ from }} for the use of delegatee {{ to }}.

{{if transfer }}
It is {{ transfer }} that I wish these tokens to become immediately owned by the delegatee.

{{/if}}
As signer I stipulate that, if I am not the beneficial owner of these tokens, I have proof that I’ve been authorized to take this action by their beneficial owner(s).

<h1 class="contract">
   newaccount
</h1>
---
spec-version: 0.0
title: Create a new account
summary: The {{ newaccount }} action creates a new account.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to exercise the authority of {{ creator }} to create a new account on this system named {{ name }} such that the new account's owner public key shall be {{ owner }} and the active public key shall be {{ active }}.

<h1 class="contract">
   refund
</h1>
---
spec-version: 0.0
title: Refund unstaked tokens
summary: The intent of the {{ refund }} action is to return previously unstaked tokens to an account after the unstaking period has elapsed.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to have the unstaked tokens of {{ owner }} returned.

<h1 class="contract">
   sellram
</h1>
---
spec-version: 0.0
title: Sell Ram
summary: The {{ sellram }} action sells unused RAM for tokens.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---
As an authorized party I {{ signer }} wish to sell {{ bytes }} of unused RAM from account {{ account }}.

<h1 class="contract">
   setprods
</h1>
---
spec-version: 0.0
title: Set new producer schedule
summary: The {{ setprods }} action creates a new schedule of active producers, who will produce blocks in the order given.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---
THIS IS A SYSTEM COMMAND NOT AVAILABLE FOR DIRECT ACCESS BY USERS.

As an authorized party I {{ signer }} wish to set the rotation of producers to be {{ schedule }}.

<h1 class="contract">
   undelegatebw
</h1>
---
spec-version: 0.0
title: Undelegate bandwidth
summary: The intent of the {{ undelegatebw }} action is to unstake tokens from CPU and/or bandwidth.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---
As an authorized party I {{ signer }} wish to unstake {{ unstake_cpu_quantity }} from CPU and {{ unstake_net_quantity }} from bandwidth from the tokens owned by {{ from }} previously delegated for the use of delegatee {{ to }}.

If I as signer am not the beneficial owner of these tokens I stipulate I have proof that I’ve been authorized to take this action by their beneficial owner(s).

<h1 class="contract">
   unregprod
</h1>
---
spec-version: 0.0
title: Unregister as Block Producer  
summary: The {{ unregprod }} action unregisters a previously registered block producer candidate.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---
As an authorized party I {{ signer }} wish to unregister the block producer candidate {{ producer }}, rendering that candidate no longer able to receive votes.

<h1 class="contract">
   voteproducer
</h1>
---
spec-version: 0.0
title: Vote for Block Producer(s)  
summary: The intent of the {{ voteproducer }} action is to cast a valid vote for up to 30 BP candidates.
icon: https://boscore.io/icon_256.png#b264b855c6d3335e5ee213443f679fb87c3633de8bc31cf66a766daac6dc6d7c
---

As an authorized party I {{ signer }} wish to vote on behalf of {{ voter }} in favor of the block producer candidates {{ producers }} with a voting weight equal to all tokens currently owned by {{ voter }} and staked for CPU or bandwidth.

If I am not the beneficial owner of these shares I stipulate I have proof that I’ve been authorized to vote these shares by their beneficial owner(s).

I stipulate I have not and will not accept anything of value in exchange for these votes, on penalty of confiscation of these tokens, and other penalties.

I acknowledge that using any system of automatic voting, re-voting, or vote refreshing, or allowing such a system to be used on my behalf or on behalf of another, is forbidden and doing so violates this contract.
