import logging

# SQLAlchemy library
import sqlalchemy
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

# SFC library
from sunnybank.database import schema

# Python library
from contextlib import contextmanager

from sunnybank.config.enviroment import DB_CONNECT_STRING

LOG = logging.getLogger("FlaskApp")



def get_engine():
    DB_ENGINE = create_engine(DB_CONNECT_STRING)

    return DB_ENGINE


def drop_table():
    engine = get_engine()

    try:
        LOG.info("Dropping all existed tables.")
        schema.DB_BASE.metadata.drop_all(engine)
        LOG.info("Done.")

    except Exception, e:
        LOG.error("Error occured when dropping tables: %s." % str(e))


def create_all_table():
    engine = get_engine()

    LOG.info("Creating tables.")
    schema.DB_BASE.metadata.create_all(engine)
    LOG.info("Done.")


def get_maker():
    engine = get_engine()

    new_maker = sessionmaker(bind=engine)

    return new_maker


@contextmanager
def init_session():
    maker = get_maker()
    session = maker()
    message = None

    try:
        message = "Start database connection."
        LOG.info(message)

        yield session
        if session.transaction is not None:
            message = "Commit data to database."
            LOG.info(message)

            session.commit()

    except sqlalchemy.exc.SQLAlchemyError as e:
        # Exception raised by SQLAlchemy
        message = "SQLAlchemy Error: %s." % str(e)
        session.rollback()
        LOG.error(message)
        raise

    except sqlalchemy.exc.DBAPIError as e:
        # Exception raised by DBAPI.
        message = "DBAPI Error: %s." % str(e)
        session.rollback()
        LOG.error(message)
        raise

    except Exception as e:
        # Other exceptions.
        message = "General Error: %s." % str(e)
        session.rollback()
        LOG.error(message)
        raise

    finally:
        message = "Complete database action, close session now."
        session.close()
        LOG.info(message)

if __name__ == "__main__":
    drop_table()
    create_all_table()
