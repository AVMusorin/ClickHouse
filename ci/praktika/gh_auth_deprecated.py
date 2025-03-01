import time

import requests
from jwt import JWT, jwk_from_pem

from .utils import Shell


# XXX: dead code with a bug in return installations[0]["id"] and using legacy jwt module
class GHAuth:
    @staticmethod
    def _generate_jwt(client_id, pem):
        pem = str.encode(pem)
        signing_key = jwk_from_pem(pem)
        payload = {
            "iat": int(time.time()),
            "exp": int(time.time()) + 600,
            "iss": client_id,
        }
        # Create JWT
        jwt_instance = JWT()
        encoded_jwt = jwt_instance.encode(payload, signing_key, alg="RS256")
        return encoded_jwt

    @staticmethod
    def _get_installation_id(jwt_token):
        headers = {
            "Authorization": f"Bearer {jwt_token}",
            "Accept": "application/vnd.github.v3+json",
        }
        response = requests.get(
            "https://api.github.com/app/installations", headers=headers, timeout=10
        )
        response.raise_for_status()
        installations = response.json()
        assert installations, "No installations found for the GitHub App"
        return installations[0]["id"]

    @staticmethod
    def _get_access_token(jwt_token, installation_id):
        headers = {
            "Authorization": f"Bearer {jwt_token}",
            "Accept": "application/vnd.github.v3+json",
        }
        url = (
            f"https://api.github.com/app/installations/{installation_id}/access_tokens"
        )
        response = requests.post(url, headers=headers, timeout=10)
        response.raise_for_status()
        return response.json()["token"]

    @classmethod
    def auth(cls, app_key, app_id) -> None:
        # Generate JWT
        jwt_token = cls._generate_jwt(app_id, app_key)
        # Get Installation ID
        installation_id = cls._get_installation_id(jwt_token)
        # Get Installation Access Token
        access_token = cls._get_access_token(jwt_token, installation_id)
        Shell.check(f"echo {access_token} | gh auth login --with-token", strict=True)
        Shell.check(f"gh auth status", strict=True)
        return access_token
