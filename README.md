# Rebcrypt

This is a simple wrapper around [node bcrypt](https://github.com/kelektiv/node.bcrypt.js) for BuckleScript and Reason.


# Getting Started

Install `rebcrypt` using npm:

```npm install --save @ncthbrt/rebcrypt```

Then add `@ncthbrt/rebcrypt` as a dependency in `bsconfig.json`.


## Examples

Bcrypt is a widely used adaptive password hashing function which also includes a mechanism to correctly salt a password. It is adaptive in that the number of rounds used to hash the password can be increased in response to an increase in computing power.

This library offers two ways of hashing a password. You can generate the salt separately and then hash the password by including the generated salt, or you can hash and salt the password at the same time. 

The library provides both asynchronous and synchronous methods.

### Generating the Salt then Hashing
```reason
/* gen() returns a promise instead */
let salt = Bcrypt.Salt.genSync(~rounds=15, ());
let hash = Bcrypt.Hash.makeSync("REALLY SECURE PASSWORD", salt);
```

### Simultaneously Salting and Hashing
```reason
let hash = hashAndSaltSync(~rounds=15, "REALLY SECURE PASSWORD");
```
