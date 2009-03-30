class Renter < ActiveRecord::Base
  #has_many :modifications
  has_many :transactions
  has_many :favorites
  belongs_to :user
end
