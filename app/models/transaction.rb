class Transaction < ActiveRecord::Base
  has_many :modifications
  belongs_to :renter
  belongs_to :host
  
end
