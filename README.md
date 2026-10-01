# ft_irc

Serveur IRC écrit en **C++98**, compatible avec les clients IRC standards (WeeChat, HexChat, irssi…).

## Fonctionnalités

- Authentification par mot de passe (`PASS`, `NICK`, `USER`)
- Gestion de plusieurs clients simultanés avec des sockets non bloquants et `poll()` [à adapter : `select` / `epoll`]
- Salons : `JOIN`, `PART`, `TOPIC`, `NAMES`
- Messages privés et messages de salon : `PRIVMSG`
- Commandes d'opérateur : `KICK`, `INVITE`, `TOPIC`, `MODE`
- Modes de salon : `i` (sur invitation), `t` (topic réservé), `k` (clé), `o` (opérateur), `l` (limite d'utilisateurs)

## Prérequis

- Un compilateur C++ compatible C++98 (`g++` / `clang++`)
- `make`

## Compilation

```bash
git clone https://github.com/FloraCornillon/irc.git
cd irc
make
```

Cibles disponibles : `make`, `make clean`, `make fclean`, `make re`.

Le projet est compilé avec `-Wall -Wextra -Werror -std=c++98`.

## Utilisation

```bash
./ircserv <port> <mot_de_passe>
```

| Argument | Description |
|---|---|
| `port` | Port d'écoute (ex. `6667`) |
| `mot_de_passe` | Mot de passe demandé aux clients |

### Test rapide avec netcat

```bash
nc -C localhost 6667
PASS motdepasse
NICK flora
USER flora 0 * :Flora
JOIN #general
PRIVMSG #general :Salut !
```

### Avec un client IRC

```bash
irssi -c localhost -p 6667 -w motdepasse -n flora
```

## Commandes supportées

| Commande | Description |
|---|---|
| `PASS` | Fournir le mot de passe du serveur |
| `NICK` | Définir ou changer de pseudo |
| `USER` | Définir le nom d'utilisateur |
| `JOIN` | Rejoindre un salon |
| `PART` | Quitter un salon |
| `PRIVMSG` | Envoyer un message à un utilisateur ou un salon |
| `KICK` | Expulser un utilisateur d'un salon |
| `INVITE` | Inviter un utilisateur dans un salon |
| `TOPIC` | Voir ou modifier le sujet d'un salon |
| `MODE` | Modifier les modes d'un salon |
| `QUIT` | Se déconnecter |

## Structure du projet

```
irc/
├── Makefile
├── includes/    # En-têtes (.hpp)
├── srcs/        # Sources (.cpp)
└── README.md
```

## Références

- [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812)

## Auteure

Flora Cornillon — [@FloraCornillon](https://github.com/FloraCornillon)
